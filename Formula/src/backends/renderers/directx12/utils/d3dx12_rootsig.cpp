#include "fm_pch.h"
#include "d3dx12_rootsig.h"

RootSignature::RootSignature(ComPtr<ID3D12Device> device, std::vector<DescLayout> rootParams)
{
	// Build root parameteres
	std::vector<CD3DX12_ROOT_PARAMETER> slotRootParameter;
	slotRootParameter.resize(rootParams.size());

	for (UINT32 i = 0; i < slotRootParameter.size(); ++i)
	{
		SetRootParameter(slotRootParameter[i], rootParams[i]);
	}
	
	// Create root signature
	CreateGraphicsRootSignature(std::move(slotRootParameter));
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
		UINT32 numOfDescs = rootParam.descLayoutSpec[0].numOfDescs;

		slotRootParameter.InitAsConstants(1, m_CBVOffset);
		m_CBVOffset += numOfDescs;
	}
}

void RootSignature::CreateGraphicsRootSignature(const std::vector<CD3DX12_ROOT_PARAMETER> slotRootParameters)
{

}
