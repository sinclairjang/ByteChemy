#pragma once

template<typename T>
static inline void SafeRelease(T*& res)
{
    if (res)
        res->Release();
    res = NULL;
}

struct SceneFrameContext
{
    SceneFrameContext() = default;

    void Init(ID3D12Device* device);

    SceneFrameContext(const SceneFrameContext& rhs) = delete;
    SceneFrameContext& operator=(const SceneFrameContext& rhs) = delete;
    ~SceneFrameContext();

    ComPtr<ID3D12CommandAllocator> SceneRenderCommandAllocator;

    UINT64 Fence = 0;

private:
    ID3D12Device* m_Device;
};

class RenderTexture
{
public:
    RenderTexture();

    void Init(DXGI_FORMAT format);

    void SetDevice(ID3D12Device* device, D3D12_CPU_DESCRIPTOR_HANDLE srvDescriptor, D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor);
    void SetWindow(const size_t width, const size_t height);
    void SetClearColor(DirectX::FXMVECTOR color);
    void ResizeResource(size_t width, size_t height);

    void ReleaseDevice() noexcept;

    void TransitionTo(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES afterState);
    void BeginScene(ID3D12GraphicsCommandList* commandList);
    void EndScene(ID3D12GraphicsCommandList* commandList);

    inline ID3D12Resource* GetResource() const noexcept { return m_Resource.Get(); }
    inline D3D12_RESOURCE_STATES GetCurrentState() const noexcept { return m_State; }
    inline DXGI_FORMAT GetFormat() const noexcept { return m_Format; }

private:
    ID3D12Device* m_Device = nullptr;
    
    ComPtr<ID3D12Resource> m_Resource;
    D3D12_RESOURCE_STATES m_State = D3D12_RESOURCE_STATE_COMMON;
    
    D3D12_CPU_DESCRIPTOR_HANDLE m_SrvDescriptor;
    D3D12_CPU_DESCRIPTOR_HANDLE m_RtvDescriptor;
    
    float m_ClearColor[4];
    DXGI_FORMAT m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    
    size_t m_Width = 0;
    size_t m_Height = 0;
};



