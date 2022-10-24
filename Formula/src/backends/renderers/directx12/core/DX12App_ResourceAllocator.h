#pragma once

#include "utils/Log.h"

ComPtr<ID3D12Resource>
DefaultBufferAllocator(ID3D12Device* device, const void* initData, UINT64 byteSize);

template<typename T>
class UploadBufAllocator
{
public:
    UploadBufAllocator(ID3D12Device* device, bool isConstantBuffer)
        : m_IsConstantBuffer(isConstantBuffer)
    {
        m_ElementByteSize = sizeof(T);

        if (m_IsConstantBuffer)
        {
            m_ElementByteSize = ((sizeof(T) + 255) % ~255);
        }
    }

    UploadBufAllocator(const UploadBufAllocator& rhs) = delete;
    UploadBufAllocator* operator=(const UploadBufAllocator& rhs) = delete;

    void UploadBufAllocator::SetNumElements(UINT64 numElements)
    {
        m_NumElement = numElement;
    }

    int UploadBufAllocator::GetElementByteSize() const
    {
        return m_ElementByteSize;
    }

    void UploadBufAllocator::Map()
    {
        if (!m_NumElement)
            LOG_INFO("Please specify allocation size")
            return
        
        ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize * m_NumElement),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(m_UploadBuffer.GetAddressOf())));  

        ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
    }

    void UploadBufAllocator::SafeRelease()
    {
        if (m_UploadBuffer != nullptr)
        {
            m_UploadBuffer->Unmap(0, nullptr);

            m_UploadBuffer->Release();  //Debug Marker
        }

        m_MappedData = nullptr;
    }

    inline ID3D12Resource* Resource() const
    {
        return m_UploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&m_MappedData[elementIndex * m_ElementByteSize], &data, sizeof(T));
    }

private:
    ComPtr<ID3D12Resource> m_UploadBuffer;
    BYTE* m_MappedData = nullptr;

    int m_ElementByteSize = 0;
    UINT64 m_NumElement = 0;
    bool m_IsConstantBuffer = false;
};

class ImageTexture
{
public:
    ImageTexture(ID3D12Device* device);
    ~ImageTexture();

    ImageTexture(const ImageTexture& rhs) = delete;
    ImageTexture* operator=(const ImageTexture& rhs) = delete;

    void CreateImageTextureFromFile(const std::wstring& path);

private:
    ID3D12Device* m_Device;
    
    ComPtr<ID3D12Resource> m_Tex2D;
    ComPtr<ID3D12DescriptorHeap> m_SrvHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE m_SrvDescriptor;
};

struct SubmeshGeometry
{
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    INT BaseVertexLocation = 0;
};

struct MeshGeometry
{
    std::string Name;

    ComPtr<ID3DBlob> VertexBufferCPU = nullptr;
    ComPtr<ID3DBlob> IndexBufferCPU = nullptr;

    ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;
    ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;

    UINT VertexByteStride = 0;
    UINT VertexBufferByteSize = 0;
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
    UINT IndexBufferByteSize = 0;

    std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

    D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
    {
        D3D12_VERTEX_BUFFER_VIEW vbv{};
        vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        vbv.StrideInBytes = VertexByteStride;
        vbv.SizeInBytes = VertexBufferByteSize;

        return vbv;
    }

    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
    {
        D3D12_INDEX_BUFFER_VIEW ibv{};
        ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        ibv.Format = IndexFormat;
        ibv.SizeInBytes = IndexBufferByteSize;

        return ibv;
    }
};