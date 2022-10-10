#pragma once

std::array<CD3DX12_STATIC_SAMPLER_DESC, 6>
GetStaticSamplers();

enum class DescLayoutType : UINT8
{
	TABLE,
	DESCRIPTOR,
	CONSTANT,
};

enum class DescType : UINT8
{
	CBV,
	SRV,
	UAV,
};

struct DescLayoutSpec
{
	DescType descType;
	UINT32 numOfDescs;
};

struct DescLayout
{
	DescLayoutType descLayoutType;
	std::vector<DescLayoutSpec> descLayoutSpec;
};

class RootSignature
{
public:
	RootSignature(ComPtr<ID3D12Device> device);

	inline ComPtr<ID3D12RootSignature> GetGraphicsRootSignature() { return m_GraphicsRootSignature; }		

private:
	void CreateGraphicsRootSignature(std::vector<DescLayout> rootParams);
	void SetRootParameter(CD3DX12_ROOT_PARAMETER& slotRootParameter, const DescLayout& rootParam);

private:
	ComPtr<ID3D12Device>		m_Device;
	ComPtr<ID3D12RootSignature> m_GraphicsRootSignature;

	UINT32 m_CBVOffset = 0;
	UINT32 m_SRVOffset = 0;
	UINT32 m_UAVOffset = 0;
};
