#include "fm_pch.h"
#include "d3dx12_rootsig.h"

#include "d3dx12_error.h"

RootSignature::RootSignature(ComPtr<ID3D12Device> device)
	: m_Device(device)
{
}

void RootSignature::CreateGraphicsRootSignature(std::vector<DescLayout> rootParams)
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

	ThrowIfFailed(m_Device->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(m_GraphicsRootSignature.GetAddressOf())));
}

void RootSignature::SetRootParameter(CD3DX12_ROOT_PARAMETER& slotRootParameter, const DescLayout& rootParam)
{
	if (rootParam.descLayoutType == DescLayoutType::TABLE)
	{
		std::vector<CD3DX12_DESCRIPTOR_RANGE> descTable;
		
		UINT descTableSize = rootParam.descLayoutSpec.size();

		descTable.resize(descTableSize);
		
		for (UINT32 i = 0; i < descTable.size(); ++i)
		{
			UINT32 numOfDescs = rootParam.descLayoutSpec[i].numOfDescs;
			
			if (rootParam.descLayoutSpec[i].descType == DescType::CBV)
			{
				descTable[i].Init(
					D3D12_DESCRIPTOR_RANGE_TYPE_CBV,
					numOfDescs,
					m_CBVOffset
				);

				m_CBVOffset += numOfDescs;
			}

			else if (rootParam.descLayoutSpec[i].descType == DescType::SRV)
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

	if (rootParam.descLayoutType == DescLayoutType::DESCRIPTOR)
	{
		UINT32 numOfDescs = rootParam.descLayoutSpec[0].numOfDescs;		// == 1
		
		if (rootParam.descLayoutSpec[0].descType == DescType::CBV)
		{
			slotRootParameter.InitAsConstantBufferView(m_CBVOffset);
			m_CBVOffset += numOfDescs;
		}

		else if (rootParam.descLayoutSpec[0].descType == DescType::SRV)
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

	if (rootParam.descLayoutType == DescLayoutType::CONSTANT)
	{
		UINT32 numOfDescs = rootParam.descLayoutSpec[0].numOfDescs;		// == 1

		slotRootParameter.InitAsConstants(1, m_CBVOffset);
		m_CBVOffset += numOfDescs;
	}
}
