#pragma once

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

    inline ID3D12Resource* GetResource() const noexcept { return m_Resource.Get(); }
    inline D3D12_RESOURCE_STATES GetCurrentState() const noexcept { return m_State; }
    inline DXGI_FORMAT GetFormat() const noexcept { return m_Format; }

private:
    ComPtr<ID3D12Device>	m_Device;
    ComPtr<ID3D12Resource>	m_Resource;
    D3D12_RESOURCE_STATES					m_State;
    D3D12_CPU_DESCRIPTOR_HANDLE				m_SrvDescriptor;
    D3D12_CPU_DESCRIPTOR_HANDLE				m_RtvDescriptor;
    float									m_ClearColor[4];
    DXGI_FORMAT								m_Format;
    size_t									m_Width;
    size_t									m_Height;
};

std::array<CD3DX12_STATIC_SAMPLER_DESC, 6>
GetStaticSamplers()
{
    CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        0,
        D3D12_FILTER_MIN_MAG_MIP_POINT,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);

    CD3DX12_STATIC_SAMPLER_DESC pointClamp(
        1,
        D3D12_FILTER_MIN_MAG_MIP_POINT,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP ,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

    CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        2,
        D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP);

    CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        3,
        D3D12_FILTER_MIN_MAG_MIP_LINEAR,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

    CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        4,
        D3D12_FILTER_ANISOTROPIC,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        D3D12_TEXTURE_ADDRESS_MODE_WRAP,
        0.0f,
        8);

    CD3DX12_STATIC_SAMPLER_DESC pointWrap(
        5,
        D3D12_FILTER_ANISOTROPIC,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
        0.0f,
        8);
}

auto staticSamplers = GetStaticSamplers();