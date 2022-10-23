#include "fm_pch.h"

#include "DX12App_ResourceAllocator.h"
#include "DX12App_ErrorHandler.h"
#include "DX12App_Synchronizer.h"

ComPtr<ID3D12Resource> 
DefaultBufferAllocator(ID3D12Device* device, const void* initData, UINT64 byteSize)
{
    ComPtr<ID3D12Resource> uploadBuffer;
    ComPtr<ID3D12Resource> defaultBuffer;
    
    WaitSync waitSync(device);
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
    
    // !Safety: Upon return, WaitSync will release the COM object of ID3D12CommandAlloc type that cmdList hold reference to. 
    return defaultBuffer;
}

ImageTexture::ImageTexture(ID3D12Device* device) :
    m_Device(device)
{
}

ImageTexture::~ImageTexture()
{
}

void ImageTexture::CreateImageTextureFromFile(const std::wstring& path)
{
    namespace fs = std::filesystem;

    std::wstring ext = fs::path(path).extension();

    DirectX::ScratchImage scratchImage;

    if (ext == L".dds" || ext == L".DDS")
        DirectX::LoadFromDDSFile(path.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);
    else if (ext == L".tga" || ext == L".TGA")
        DirectX::LoadFromTGAFile(path.c_str(), nullptr, scratchImage);
    else  // bmp, png, giff, tiff, jpeg
        DirectX::LoadFromWICFile(path.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratchImage);

    ThrowIfFailed(DirectX::CreateTexture(
        m_Device,
        scratchImage.GetMetadata(),
        m_Tex2D.GetAddressOf()));


    std::vector<D3D12_SUBRESOURCE_DATA> subResources;

    ThrowIfFailed(DirectX::PrepareUpload(
        m_Device,
        scratchImage.GetImages(),
        scratchImage.GetImageCount(),
        scratchImage.GetMetadata(),
        subResources));

    const UINT64 bufSize = ::GetRequiredIntermediateSize(
        m_Tex2D.Get(),
        0,
        static_cast<UINT>(subResources.size()));

    D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    D3D12_RESOURCE_DESC rscDesc = CD3DX12_RESOURCE_DESC::Buffer(bufSize);

    ComPtr<ID3D12Resource> texUploadBuf;
    
    WaitSync waitSync(m_Device);
    auto cmdList = waitSync.Begin();

    ThrowIfFailed(m_Device->CreateCommittedResource(
        &heapProperty,
        D3D12_HEAP_FLAG_NONE,
        &rscDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(texUploadBuf.GetAddressOf())));

    ::UpdateSubresources(
        cmdList,
        m_Tex2D.Get(),
        texUploadBuf.Get(),
        0, 0,
        static_cast<UINT>(subResources.size()),
        subResources.data());

    waitSync.Flush();

    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 1;
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    ThrowIfFailed(m_Device->CreateDescriptorHeap(
        &srvHeapDesc,
        IID_PPV_ARGS(m_SrvHeap.GetAddressOf())
    ));

    m_SrvDescriptor = m_SrvHeap->GetCPUDescriptorHandleForHeapStart();

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = scratchImage.GetMetadata().format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MipLevels = 1;
    m_Device->CreateShaderResourceView(
        m_Tex2D.Get(),
        &srvDesc,
        m_SrvDescriptor
    );
}
