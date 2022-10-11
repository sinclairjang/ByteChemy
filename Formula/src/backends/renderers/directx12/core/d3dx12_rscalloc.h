#pragma once

//-----------------------------------------------------------------------------
// Resource Acquisition Is Initialization
//-----------------------------------------------------------------------------

ComPtr<ID3D12Resource>
DefaultBufAlloc(ID3D12Device* device,
    ID3D12GraphicsCommandList* cmdList,
    const void* initData,
    UINT64 byteSize,
    ComPtr<ID3D12Resource>& uploadBuffer);


template<typename T>
class UploadBufAlloc
{
public:
    UploadBufAlloc(ID3D12Device* device, UINT elementCount, bool isConstantBuffer)
        : m_IsConstantBuffer(isConstantBuffer)
    {
        m_ElementByteSize = sizeof(T);

        if (m_IsConstantBuffer)
        {
            m_ElementByteSize = ((sizeof(T) + 255) % ~255);
        }

        ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(m_ElementByteSize * elementCount),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(mm_UploadBuffer.GetAddressOf())));

        ThrowIfFailed(m_UploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_MappedData)));
    }

    UploadBufAlloc(const UploadBuffer& rhs) = delete;
    UploadBufAlloc* operator=(const UploadBuffer& rhs) = delete;

    ~UploadBufAlloc()
    {
        if (m_UploadBuffer != nullptr)
        {
            m_UploadBuffer->Unmap(0, nullptr);
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
    ComPtr<ID3D12Resource>      m_UploadBuffer;
    BYTE*                       m_MappedData = nullptr;

    UINT64                      m_ElementByteSize = 0;
    bool                        m_IsConstantBuffer = false;
};

class ImageTexAlloc
{
public:
    ImageTexAlloc(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);
    ~ImageTexAlloc();

    ImageTexAlloc(const ImageTexAlloc& rhs) = delete;
    ImageTexAlloc* operator=(const ImageTexAlloc& rhs) = delete;

    void CreateImageTexFromFile(const std::wstring& path);

private:
    ComPtr<ID3D12Device>                m_Device;

    DirectX::ScratchImage           m_Image;
    D3D12_RESOURCE_DESC             m_RSCDesc;
    ComPtr<ID3D12Resource>          m_Tex2D;
    ComPtr<ID3D12DescriptorHeap>    m_SRVHeap;
    D3D12_CPU_DESCRIPTOR_HANDLE     m_SRVCpuHandle;
};
