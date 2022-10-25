#pragma once

#include "DX12App_RootSigner.h"

enum class RENDER_MODE : UINT8
{
	DEFERRED,
	FORWARD,
};

enum class RASTERIZER_TYPE : UINT8
{
	CULL_NONE,
	CULL_FRONT,
	CULL_BACK,
	WIREFRANE,
};

enum class DEPTH_STENCIL_COMPARISON_FUNC_TYPE : UINT8
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_DEPTH_TEST,
	NO_DEPTH_TEST_NO_WRITE,
	LESS_NO_WRITE,
};

enum class BLEND_TYPE : UINT8
{
	DEFAULT,
	ALPHA_BLEND,
	ONE_TO_ONE_BLEND,
};

// Shader Naming Convention
struct MainFuncNames
{
	std::string VS = "VS_Main";
	std::string HS;
	std::string DS;
	std::string GS;
	std::string PS = "PS_Main";
};

struct GPUPipelineSpecification
{
	RENDER_MODE RenderMode = RENDER_MODE::FORWARD;
	RASTERIZER_TYPE RasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_COMPARISON_FUNC_TYPE DSCompFuncType = DEPTH_STENCIL_COMPARISON_FUNC_TYPE::LESS;
	BLEND_TYPE BlendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY PrimTopologyType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	MainFuncNames ShaderNames = MainFuncNames();
	
	GPUPipelineSpecification() = default;
	GPUPipelineSpecification(const GPUPipelineSpecification& other) = default;
	
	GPUPipelineSpecification(
		RENDER_MODE renderMode,
		RASTERIZER_TYPE rasterizerType,
		DEPTH_STENCIL_COMPARISON_FUNC_TYPE dsCompFuncType,
		BLEND_TYPE blendType,
		D3D_PRIMITIVE_TOPOLOGY primTopologyType,
		MainFuncNames shaderNames)
	{
		RenderMode = renderMode;
		RasterizerType = rasterizerType;
		DSCompFuncType = dsCompFuncType;
		BlendType = blendType;
		PrimTopologyType = primTopologyType;
		ShaderNames = shaderNames;
	}
	
	static inline GPUPipelineSpecification Primitive(
		D3D_PRIMITIVE_TOPOLOGY primTopologyType)
	{
		return GPUPipelineSpecification(
			RENDER_MODE::FORWARD, 
			RASTERIZER_TYPE::CULL_BACK,
			DEPTH_STENCIL_COMPARISON_FUNC_TYPE::LESS,
			BLEND_TYPE::DEFAULT,
			primTopologyType,
			MainFuncNames()
		);
	}
};

class Plumber
{
public:
	Plumber(ID3D12Device* device, const RootSignature* rootSig);
	~Plumber();

	void CreateGraphicsShader(const std::wstring& path, GPUPipelineSpecification pipeSpec);
	inline ComPtr<ID3D12PipelineState> GetGraphicsPipelineHandle() { return m_GraphicsPipelineState; }
	 

private:
	void CreateShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version, 
		ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateHullShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateDomainShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateGeometryShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

	D3D12_PRIMITIVE_TOPOLOGY_TYPE GetPrimTopologyType(D3D_PRIMITIVE_TOPOLOGY topology);

private:
	ID3D12Device* g_Device;
	ComPtr<ID3D12PipelineState> m_GraphicsPipelineState;

	const RootSignature* m_GraphicsRootSignature;
	GPUPipelineSpecification m_PipelineSpec;

	ComPtr<ID3DBlob> m_VSBlob;
	ComPtr<ID3DBlob> m_HSBlob;
	ComPtr<ID3DBlob> m_DSBlob;
	ComPtr<ID3DBlob> m_GSBlob;
	ComPtr<ID3DBlob> m_PSBlob;
	ComPtr<ID3DBlob> m_ErrorBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_GraphicsPipelineStateDesc;
};
