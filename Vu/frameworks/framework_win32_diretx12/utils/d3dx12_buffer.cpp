#include "vu_pch.h"

#include "d3dx12_buffer.h"
#include "d3dx12_error.h"

Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(
    ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer)
{
    Microsoft::WRL::ComPtr<ID3D12Resource> defaultBuffer;

    // Create the actual default buffer
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(byteSize),
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

    // Create an intermediate upload buffer
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(byteSize),
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(uploadBuffer.GetAddressOf())));

    // Describe the data we want to copy into the default buffer
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = initData;
    subResourceData.RowPitch = byteSize;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    // Schedule to copy the data to the default bufffer
    cmdList->ResourceBarrier(1,
        &CD3DX12_RESOURCE_BARRIER::Transition(
            defaultBuffer.Get(),
            D3D12_RESOURCE_STATE_COMMON,
            D3D12_RESOURCE_STATE_COPY_DEST));

    UpdateSubresources<1>(cmdList,
        defaultBuffer.Get(),
        uploadBuffer.Get(),
        0, 0, 1,
        &subResourceData);

    cmdList->ResourceBarrier(1,
        &CD3DX12_RESOURCE_BARRIER::Transition(
            defaultBuffer.Get(),
            D3D12_RESOURCE_STATE_COPY_DEST,
            D3D12_RESOURCE_STATE_GENERIC_READ));

    return defaultBuffer;
}

void RenderTexture::SetDevice(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor, D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor)
{
    if (device == m_device.Get() && srvDescriptor.ptr == m_srvDescriptor.ptr && rtvDescriptor.ptr == m_rtvDescriptor.ptr)
        return;

    if (m_device)
    {
        ReleaseDevice();
    }

    {
        D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport = { m_format, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };
        if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport))))
        {
            throw std::runtime_error("CheckFeatureSupport");
        }

        UINT required = D3D12_FORMAT_SUPPORT1_TEXTURE2D | D3D12_FORMAT_SUPPORT1_RENDER_TARGET;
        if ((formatSupport.Support1 & required) != required)
        {
#ifdef _DEBUG
            char buff[128] = {};
            sprintf_s(buff, "RenderTexture: Device does not support the requested format (%u)!\n", m_format);
            OutputDebugStringA(buff);
#endif
            throw std::runtime_error("RenderTexture");
        }
    }

    if (!srvDescriptor.ptr || !rtvDescriptor.ptr)
    {
        throw std::runtime_error("Invalid descriptors");
    }

    m_device = device;

    m_srvDescriptor = srvDescriptor;
    m_rtvDescriptor = rtvDescriptor;
}

void RenderTexture::SetWindow(const RECT& output)
{
    // Determine the render target size in pixels
    auto width = size_t(std::max<LONG>(output.right - output.left, 1));
    auto height = size_t(std::max<LONG>(output.bottom - output.top, 1));

    ResizeResource(width, height);
}

void RenderTexture::SetClearColor(DirectX::FXMVECTOR color)
{
    DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(m_clearColor), color);
}

void RenderTexture::ResizeResource(size_t width, size_t height)
{
    if (width == m_width && height == m_height)
        return;

    if (m_width > UINT32_MAX || m_height > UINT32_MAX)
    {
        throw std::runtime_error("Invalid size");
    }

    if (!m_device)
        return;

    m_width = m_height = 0;

    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(
        m_format,
        static_cast<UINT64>(width), static_cast<UINT>(height),
        1, 1, 1, 0,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    D3D12_CLEAR_VALUE clearValue = { m_format, {} };
    memcpy(clearValue.Color, m_clearColor, sizeof(clearValue.Color));

    m_state = D3D12_RESOURCE_STATE_RENDER_TARGET;

    // Create a render target
    ThrowIfFailed(
        m_device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
            &desc,
            m_state,
            &clearValue,
            IID_PPV_ARGS(m_resource.ReleaseAndGetAddressOf())));

    SetDebugObjectName(m_resource.Get(), "RenderTexture RT");

    // Create RTV
    m_device->CreateRenderTargetView(m_resource.Get(), nullptr, m_rtvDescriptor);

    // Create SRV
    m_device->CreateShaderResourceView(m_resource.Get(), nullptr, m_srvDescriptor);

    m_width = width;
    m_height = height;
}

void RenderTexture::ReleaseDevice() noexcept
{
    m_resource.Reset();
    m_device.Reset();

    m_state = D3D12_RESOURCE_STATE_COMMON;
    m_width = m_height = 0;

    m_srvDescriptor.ptr = m_rtvDescriptor.ptr = 0;
}

void RenderTexture::TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES afterState)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_resource.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = m_state;
    barrier.Transition.StateAfter = afterState;

    commandList->ResourceBarrier(1, &barrier);

    m_state = afterState;
}

void RenderTexture::BeginScene(ID3D12GraphicsCommandList* commandList)
{
    TransitionTo(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void RenderTexture::EndScene(ID3D12GraphicsCommandList* commandList)
{
    TransitionTo(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
