#include "Vu_pch.h"
#include "d3dx12_texrsc.h"

#include "d3dx12_error.h"

void RenderTexture::SetDevice(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor, D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor)
{
    if (device == m_Device.Get() && srvDescriptor.ptr == m_SrvDescriptor.ptr && rtvDescriptor.ptr == m_RtvDescriptor.ptr)
        return;

    if (m_Device)
    {
        ReleaseDevice();
    }

    {
        D3D12_FEATURE_DATA_FORMAT_SUPPORT formatSupport = { m_Format, D3D12_FORMAT_SUPPORT1_NONE, D3D12_FORMAT_SUPPORT2_NONE };
        if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &formatSupport, sizeof(formatSupport))))
        {
            throw std::runtime_error("CheckFeatureSupport");
        }

        UINT required = D3D12_FORMAT_SUPPORT1_TEXTURE2D | D3D12_FORMAT_SUPPORT1_RENDER_TARGET;
        if ((formatSupport.Support1 & required) != required)
        {
#ifdef _DEBUG
            char buff[128] = {};
            sprintf_s(buff, "RenderTexture: Device does not support the requested format (%u)!\n", m_Format);
            OutputDebugStringA(buff);
#endif
            throw std::runtime_error("RenderTexture");
        }
    }

    if (!srvDescriptor.ptr || !rtvDescriptor.ptr)
    {
        throw std::runtime_error("Invalid descriptors");
    }

    m_Device = device;

    m_SrvDescriptor = srvDescriptor;
    m_RtvDescriptor = rtvDescriptor;
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
    DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(m_ClearColor), color);
}

void RenderTexture::ResizeResource(size_t width, size_t height)
{
    if (width == m_Width && height == m_Height)
        return;

    if (m_Width > UINT32_MAX || m_Height > UINT32_MAX)
    {
        throw std::runtime_error("Invalid size");
    }

    if (!m_Device)
        return;

    m_Width = m_Height = 0;

    D3D12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Tex2D(
        m_Format,
        static_cast<UINT64>(width), static_cast<UINT>(height),
        1, 1, 1, 0,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    D3D12_CLEAR_VALUE clearValue = { m_Format, {} };
    memcpy(clearValue.Color, m_ClearColor, sizeof(clearValue.Color));

    m_State = D3D12_RESOURCE_STATE_RENDER_TARGET;

    // Create a render target
    ThrowIfFailed(
        m_Device->CreateCommittedResource(
            &heapProperties,
            D3D12_HEAP_FLAG_ALLOW_ALL_BUFFERS_AND_TEXTURES,
            &desc,
            m_State,
            &clearValue,
            IID_PPV_ARGS(m_Resource.ReleaseAndGetAddressOf())));

    SetDebugObjectName(m_Resource.Get(), "RenderTexture RT");

    // Create RTV
    m_Device->CreateRenderTargetView(m_Resource.Get(), nullptr, m_RtvDescriptor);

    // Create SRV
    m_Device->CreateShaderResourceView(m_Resource.Get(), nullptr, m_SrvDescriptor);

    m_Width = width;
    m_Height = height;
}

void RenderTexture::ReleaseDevice() noexcept
{
    m_Resource.Reset();
    m_Device.Reset();

    m_State = D3D12_RESOURCE_STATE_COMMON;
    m_Width = m_Height = 0;

    m_SrvDescriptor.ptr = m_RtvDescriptor.ptr = 0;
}

void RenderTexture::TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES afterState)
{
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_Resource.Get();
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = m_State;
    barrier.Transition.StateAfter = afterState;

    commandList->ResourceBarrier(1, &barrier);

    m_State = afterState;
}

void RenderTexture::BeginScene(ID3D12GraphicsCommandList* commandList)
{
    TransitionTo(commandList, D3D12_RESOURCE_STATE_RENDER_TARGET);
}

void RenderTexture::EndScene(ID3D12GraphicsCommandList* commandList)
{
    TransitionTo(commandList, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
