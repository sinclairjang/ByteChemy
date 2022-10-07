#pragma once

enum class SHADER_TYPE : UINT8
{
	DEFERRED,
	FORWARD,
	LIGHTING,
	PARTICLE,
	SHADOW,
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

struct ShaderSpec
{

	struct MainFuncName
	{
		std::string VS = "VS";
		std::string HS;
		std::string DS;
		std::string GS;
		std::string PS = "PS";
	};

	SHADER_TYPE shaderType = SHADER_TYPE::FORWARD;
	RASTERIZER_TYPE rasterizerType = RASTERIZER_TYPE::CULL_BACK;
	DEPTH_STENCIL_COMPARISON_FUNC_TYPE dsCompFuncType = DEPTH_STENCIL_COMPARISON_FUNC_TYPE::LESS;
	BLEND_TYPE blendType = BLEND_TYPE::DEFAULT;
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	MainFuncName mainFuncName = MainFuncName();
};

class Shader
{
public:
	Shader(ID3D12Device* device, const std::wstring& path, ShaderSpec spec = ShaderSpec());
	~Shader();

	Shader(const Shader& other) = delete;
	Shader& operator=(const Shader& other) = delete;

	void CreateGraphicsShader(ID3D12Device* device, const std::wstring& path, ShaderSpec spec = ShaderSpec());
	
private:
	void CreateShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version, 
		ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateHullShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateDomainShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreateGeometryShader(const std::wstring& path, const std::string& name, const std::string& version);
	void CreatePixelShader(const std::wstring& path, const std::string& name, const std::string& version);

private:
	ShaderSpec m_ShaderSpec;
	ComPtr<ID3D12PipelineState> m_PipelineState;

	ComPtr<ID3DBlob> m_VSBlob;
	ComPtr<ID3DBlob> m_HSBlob;
	ComPtr<ID3DBlob> m_DSBlob;
	ComPtr<ID3DBlob> m_GSBlob;
	ComPtr<ID3DBlob> m_PSBlob;
	ComPtr<ID3DBlob> m_ErrorBlob;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC m_GraphicsPipelineStateDesc;
};
