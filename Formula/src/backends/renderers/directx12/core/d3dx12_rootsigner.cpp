#include "fm_pch.h"
#include "d3dx12_rootsigner.h"

#include "d3dx12_error.h"

RootSignature::RootSignature(ID3D12Device* device)
	: g_Device(device)
{
}

void RootSignature::CreateGraphicsRootSignature(std::vector<RootParmeter>& rootParams)
{
	// Build root parameteres
	std::vector<CD3DX12_ROOT_PARAMETER> slotRootParameters;
	slotRootParameters.resize(rootParams.size());

	for (UINT32 i = 0; i < slotRootParameters.size(); ++i)
	{
		SetRootParameter(slotRootParameters[i], rootParams[i]);
	}
	
	auto staticSamplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameters.data(),
		(UINT)staticSamplers.size(), staticSamplers.data(),
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	ThrowIfFailed(hr);

	ThrowIfFailed(g_Device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(m_GraphicsRootSignature.GetAddressOf())));
}

void RootSignature::SetRootParameter(CD3DX12_ROOT_PARAMETER& slotRootParameter, const RootParmeter& rootParam)
{
	if (rootParam.Layout == LeafParametersLayout::TABLE)
	{
		std::vector<CD3DX12_DESCRIPTOR_RANGE> descTable;
		
		UINT descTableSize = (UINT)rootParam.DescArrays.size();

		descTable.resize(descTableSize);
		
		for (UINT32 i = 0; i < descTable.size(); ++i)
		{
			UINT32 numOfDescs = rootParam.DescArrays[i].NumLeafParameters;
			
			if (rootParam.DescArrays[i].Type == LeafParameterType::CBV)
			{
				descTable[i].Init(
					D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
					numOfDescs,
					m_CBVOffset
				);

				m_CBVOffset += numOfDescs;
			}

			else if (rootParam.DescArrays[i].Type == LeafParameterType::SRV)
			{
				descTable[i].Init(
					D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
					numOfDescs,
					m_SRVOffset
				);

				m_SRVOffset += numOfDescs;
			}

			else
			{
				descTable[i].Init(
					D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
					numOfDescs,
					m_UAVOffset
				);

				m_UAVOffset += numOfDescs;
			}
		}

		slotRootParameter.InitAsDescriptorTable(
			descTableSize,
			descTable.data()
		);
	}

	if (rootParam.Layout == LeafParametersLayout::DESCRIPTOR)
	{
		UINT32 numOfDescs = rootParam.DescArrays[0].NumLeafParameters;		// == 1
		
		if (rootParam.DescArrays[0].Type == LeafParameterType::CBV)
		{
			slotRootParameter.InitAsConstantBufferView(m_CBVOffset);
			m_CBVOffset += numOfDescs;
		}

		else if (rootParam.DescArrays[0].Type == LeafParameterType::SRV)
		{
			slotRootParameter.InitAsShaderResourceView(m_SRVOffset);
			m_SRVOffset += numOfDescs;
		}

		else
		{
			slotRootParameter.InitAsUnorderedAccessView(m_UAVOffset);
			m_UAVOffset += numOfDescs;
		}
	}

	if (rootParam.Layout == LeafParametersLayout::CONSTANT)
	{
		UINT32 numOfDescs = rootParam.DescArrays[0].NumLeafParameters;

		slotRootParameter.InitAsConstants(numOfDescs, m_CBVOffset);
		m_CBVOffset += 1;
	}
}

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
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

	CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP);

	CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

	CD3DX12_STATIC_SAMPLER_DESC anistropicWrap(
		4,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		0.0f,
		8);

	CD3DX12_STATIC_SAMPLER_DESC anistropicClamp(
		5,
		D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		0.0f,
		8);

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anistropicWrap, anistropicClamp };
}