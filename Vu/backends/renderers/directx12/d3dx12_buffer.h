#pragma once

Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device,
        ID3D12GraphicsCommandList* cmdList,
        const void* initData,
        UINT64 byteSize,
        Microsoft::WRL::ComPtr<ID3D12Resource>& uploadBuffer);

template<typename T>
class UploadBuffer
{
public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer)
        : _isConstantBuffer(isConstantBuffer)
    {
        _elementByteSize = sizeof(T);

        if (_isConstantBuffer)
        {
            _elementByteSize = ((sizeof(T) + 255) % ~255);
        }

        ThrowIfFailed(device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(_elementByteSize * elementCount),
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(_uploadBuffer.GetAddressOf())));

        ThrowIfFailed(_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
    }

    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer* operator=(const UploadBuffer& rhs) = delete;

    ~UploadBuffer()
    {
        if (_uploadBuffer != nullptr)
        {
            _uploadBuffer->Unmap(0, nullptr);
        }

        _mappedData = nullptr;
    }

    inline ID3D12Resource* Resource() const
    {
        return _uploadBuffer.Get();
    }

    void CopyData(int elementIndex, const T& data)
    {
        memcpy(&_mappedData[elementIndex * _elementByteSize], &data, sizeof(T));
    }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> _uploadBuffer;
    BYTE* _mappedData = nullptr;

    UINT64 _elementByteSize = 0;
    bool _isConstantBuffer = false;
};

class RenderTexture
{
public:
    RenderTexture(DXGI_FORMAT format) noexcept;
    
    void SetDevice(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor, D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor);
    void SetWindow(const RECT& output);
    void SetClearColor(DirectX::FXMVECTOR color);
    void ResizeResource(size_t width, size_t height);

    void ReleaseDevice() noexcept;
    
    void TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES afterState);
    void BeginScene(ID3D12GraphicsCommandList* commandList);
    void EndScene(ID3D12GraphicsCommandList* commandList);

    inline ID3D12Resource* GetResource() const noexcept { return m_resource.Get(); }
    inline D3D12_RESOURCE_STATES GetCurrentState() const noexcept { return m_state; }
    inline DXGI_FORMAT GetFormat() const noexcept { return m_format; }

private:
    Microsoft::WRL::ComPtr<ID3D12Device>	m_device;
    Microsoft::WRL::ComPtr<ID3D12Resource>	m_resource;
    D3D12_RESOURCE_STATES					m_state;
    D3D12_CPU_DESCRIPTOR_HANDLE				m_srvDescriptor;
    D3D12_CPU_DESCRIPTOR_HANDLE				m_rtvDescriptor;
    float									m_clearColor[4];
    DXGI_FORMAT								m_format;
    size_t									m_width;
    size_t									m_height;
};