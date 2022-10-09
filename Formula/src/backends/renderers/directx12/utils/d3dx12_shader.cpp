#include "fm_pch.h"
#include "d3dx12_shader.h"

#include "d3dx12_error.h"
#include "d3dx12_rootsig.h"

Shader::Shader(ComPtr<ID3D12Device> device, std::shared_ptr<RootSignature> rootSig)
	: m_Device(device), m_GraphicsRootSignature(rootSig)
{
}

Shader::~Shader()
{
}

void Shader::CreateGraphicsShader(const std::wstring& path, ShaderSpec shaderSpec)
{
	m_ShaderSpec = shaderSpec;

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
	m_GraphicsPipelineStateDesc.pRootSignature = m_GraphicsRootSignature->GetGraphicsRootSignature().Get();
	m_GraphicsPipelineStateDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	m_GraphicsPipelineStateDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	m_GraphicsPipelineStateDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	m_GraphicsPipelineStateDesc.SampleMask = UINT_MAX;
	m_GraphicsPipelineStateDesc.PrimitiveTopologyType = GetPrimTopologyType(m_ShaderSpec.primTopologyType);
	m_GraphicsPipelineStateDesc.NumRenderTargets = 1;
	m_GraphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_GraphicsPipelineStateDesc.SampleDesc.Count = 1;
	m_GraphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	switch (m_ShaderSpec.shaderType)
	{
	case SHADER_TYPE::FORWARD:
		m_GraphicsPipelineStateDesc.NumRenderTargets = 1;
		m_GraphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		break;
	case SHADER_TYPE::DEFERRED:
		FM_ASSERTM(0, "Not supported yet!");
	case SHADER_TYPE::LIGHTING:
		FM_ASSERTM(0, "Not supported yet!");
	case SHADER_TYPE::PARTICLE:
		FM_ASSERTM(0, "Not supported yet!");
	case SHADER_TYPE::SHADOW:
		FM_ASSERTM(0, "Not supported yet!");
	}

	switch (m_ShaderSpec.rasterizerType)
	{
	case RASTERIZER_TYPE::CULL_BACK:
		m_GraphicsPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_GraphicsPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;
	case RASTERIZER_TYPE::CULL_FRONT:
		m_GraphicsPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_GraphicsPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;
	case RASTERIZER_TYPE::CULL_NONE:
		m_GraphicsPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
		m_GraphicsPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	case RASTERIZER_TYPE::WIREFRANE:
		m_GraphicsPipelineStateDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
		m_GraphicsPipelineStateDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}

	switch (m_ShaderSpec.dsCompFuncType)
	{
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::LESS:
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		break;
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::LESS_EQUAL :
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
		break;
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::GREATER:
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
		break;
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::GREATER_EQUAL:
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		break;
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::NO_DEPTH_TEST:
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_NEVER;
		break;
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::NO_DEPTH_TEST_NO_WRITE:
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = FALSE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	case DEPTH_STENCIL_COMPARISON_FUNC_TYPE::LESS_NO_WRITE:
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthEnable = TRUE;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		m_GraphicsPipelineStateDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		break;
	}

	D3D12_RENDER_TARGET_BLEND_DESC& rtBlendDesc = m_GraphicsPipelineStateDesc.BlendState.RenderTarget[0];

	switch (m_ShaderSpec.blendType)
	{
	case BLEND_TYPE::DEFAULT:
		rtBlendDesc.BlendEnable = FALSE;
		rtBlendDesc.LogicOpEnable = FALSE;
		rtBlendDesc.SrcBlend = D3D12_BLEND_ONE;
		rtBlendDesc.DestBlend = D3D12_BLEND_ZERO;
		break;
	case BLEND_TYPE::ALPHA_BLEND:
		rtBlendDesc.BlendEnable = TRUE;
		rtBlendDesc.LogicOpEnable = FALSE;
		rtBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
		rtBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	case BLEND_TYPE::ONE_TO_ONE_BLEND:
		rtBlendDesc.BlendEnable = TRUE;
		rtBlendDesc.LogicOpEnable = FALSE;
		rtBlendDesc.SrcBlend = D3D12_BLEND_ONE;
		rtBlendDesc.DestBlend = D3D12_BLEND_ONE;
		break;
	}

	ThrowIfFailed(m_Device->CreateGraphicsPipelineState(&m_GraphicsPipelineStateDesc, IID_PPV_ARGS(m_PipelineState.GetAddressOf())));
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE Shader::GetPrimTopologyType(D3D_PRIMITIVE_TOPOLOGY topology)
{
	switch (topology)
	{
	case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
	case D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP:
	case D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ:
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	case D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST:
	case D3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH;
	default:
		return D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;

	}
}