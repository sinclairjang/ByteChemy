#pragma once

std::array<CD3DX12_STATIC_SAMPLER_DESC, 6>
GetStaticSamplers();

enum class LeafParametersLayout : UINT8
{
	TABLE,
	DESCRIPTOR,
	CONSTANT,
};

enum class LeafParameterType : UINT8
{
	CBV,
	SRV,
	UAV,
	BIT32,
};

struct LeafParameterArray
{
	LeafParameterType Type;
	UINT32 NumLeafParameters;
};

// Naming Convention:
//	(e.g.) RootSignature RS_TC2S1_DC1_DS1_K4
//
// This corresponds to the following data structure.
//
// Layout:  
// { 
//	 Layout Size : 4,
//
//		Type : Table
//			CBV : 2 (b0)
//			SRV : 1 (t0)
// 
//		Type : Descriptor
//			CBV : 1 (b2)
// 
//		Type : Descriptor
//			SRV : 1 (t1)
// 
//		Type : Constant
//			32-BIT : 4 (b3)
// }

struct RootParmeter
{
	LeafParametersLayout Layout;
	std::vector<LeafParameterArray> Arrangement;
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
