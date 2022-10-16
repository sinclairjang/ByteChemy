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

/*
 RootSignature Code Name:
	(e.g.) ROOT:TC2S1,DC1,DS1,K4

 This should correspond to the following data structure.

 //TODO: Serialze/Deserialize this layout to support custom shader programming
 
 Layout:  
 { 
	 Layout Size : 4,

		Type : Table	
			CBV : 2 (b0)
			SRV : 1 (t0)
 
		Type : Descriptor
			CBV : 1 (b2)
 
		Type : Descriptor
			SRV : 1 (t1)
 
		Type : Constant
			32-BIT : 4 (b3)
 } 
 */

struct RootParmeter
{
	LeafParametersLayout Layout;
	std::vector<LeafParameterArray> DescArrays;
};

class RootSignature
{
public:
	RootSignature(ID3D12Device* device);

	void CreateGraphicsRootSignature(std::vector<RootParmeter>& rootParams);
	
	inline ComPtr<ID3D12RootSignature> GetGraphicsRootSignature() { return m_GraphicsRootSignature; }		

private:
	void SetRootParameter(CD3DX12_ROOT_PARAMETER& slotRootParameter, const RootParmeter& rootParam);

private:
	ComPtr<ID3D12Device>		m_Device;
	ComPtr<ID3D12RootSignature> m_GraphicsRootSignature;

	UINT32 m_CBVOffset = 0;
	UINT32 m_SRVOffset = 0;
	UINT32 m_UAVOffset = 0;
};
