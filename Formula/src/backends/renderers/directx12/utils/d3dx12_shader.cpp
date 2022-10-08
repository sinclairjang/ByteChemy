#include "fm_pch.h"
#include "d3dx12_shader.h"

Shader::Shader(ComPtr<ID3D12Device> device, const std::wstring& path, ShaderSpec shaderSpec)
{
	CreateGraphicsShader(path, shaderSpec);
}

Shader::~Shader()
{
}

void Shader::CreateGraphicsShader(const std::wstring& path, ShaderSpec spec)
{
	m_ShaderSpec = spec;

	CreateVertexShader(path, m_ShaderSpec.mainFuncName.VS, "vs_5_0");
	CreatePixelShader(path, m_ShaderSpec.mainFuncName.PS, "ps_5_0");

	if (m_ShaderSpec.mainFuncName.HS.empty() == false)
	{
		CreateHullShader(path, m_ShaderSpec.mainFuncName.HS, "hs_5_0");
	}

	if (m_ShaderSpec.mainFuncName.DS.empty() == false)
	{
		CreateDomainShader(path, m_ShaderSpec.mainFuncName.DS, "ds_5_0");
	}

	if (m_ShaderSpec.mainFuncName.GS.empty() == false)
	{
		CreateGeometryShader(path, m_ShaderSpec.mainFuncName.GS, "gs_5_0");
	}
	
	D3D12_INPUT_ELEMENT_DESC elemDesc[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

	m_GraphicsPipelineStateDesc.InputLayout = { elemDesc, _countof(elemDesc) };
	m_GraphicsPipelineStateDesc.pRootSignature = 

}

