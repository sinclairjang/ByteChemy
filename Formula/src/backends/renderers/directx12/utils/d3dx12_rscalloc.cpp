#include "fm_pch.h"

#include "d3dx12_rscalloc.h"
#include "d3dx12_error.h"
#include "d3dx12_sync.h"

ComPtr<ID3D12Resource> DefaultBufAlloc(
    ID3D12Device* device,
    const void* initData,
    UINT64 byteSize)
{
    WaitSync waitSync;
    ComPtr<ID3D12Resource> uploadBuffer;
    ComPtr<ID3D12Resource> defaultBuffer;
    
    auto cmdList = waitSync.Begin();

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

    waitSync.Flush();
    
    // Dangling issue has been taken care of by WaitSync 
    FM_ASSERT(cmdList == nullptr);

    return defaultBuffer;
}

ImageTexAlloc::ImageTexAlloc(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList) :
    m_Device(device), m_CmdList(cmdList)
{
}

ImageTexAlloc::~ImageTexAlloc()
{
}

void ImageTexAlloc::CreateImageTexFromFile(const std::wstring& path)
{
    namespace fs = std::filesystem;

    std::wstring ext = fs::path(path).extension();


    if (ext == L".dds" || ext == L".DDS")
        DirectX::LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, m_Image);
    else if (ext == L".tga" || ext == L".TGA")
        DirectX::LoadFromTGAFile(path.c_str(), nullptr, m_Image);
    else  // bmp, png, giff, tiff, jpeg
        DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, m_Image);

    HRESULT hr = DirectX::CreateTexture(m_Device.Get(), m_Image.GetMetadata(), m_Tex2D.GetAddressOf());

    if (FAILED(hr))
        FM_ASSERT(nullptr);

    m_RSCDesc = m_Tex2D->GetDesc();

    std::vector<D3D12_SUBRESOURCE_DATA> subResources;

    hr = DirectX::PrepareUpload()
}
