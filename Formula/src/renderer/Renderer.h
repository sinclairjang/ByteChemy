#pragma once

// Grahpics Device Basic Functionalities
namespace GraphicsService
{
	enum class GrpahicsAPI : UINT8
	{
		DirectX9,
		DirectX10,
		DirectX11,
		DirectX12,
		OPENGL2,
		OPENGL3,
		VULKAN,
		METAL,
	};

	enum class AllocateGPUMemory : UINT8
	{
		MESH,
		CONSTANT,
		TEXTURE,
	};

	enum class CreateGPUProgram : UINT8
	{
		GRAPHICS,
		COMPUTE,
	};
	
	enum class SetRenderTarget : UINT8
	{
		FRAMEBUFFER,
		TEXTURE,
	};

	enum class Render : UINT8
	{
		DIRECT,
		INSTANCED,
	};
};

class Renderer
{
public:
	virtual ~Renderer() {}

	// Graphics API Overloads
	virtual void RequestService(GraphicsService::AllocateGPUMemory allocWhat, const std::wstring& path, const void* initData,  void* outInfo) = 0;
	virtual void RequestService(GraphicsService::CreateGPUProgram shaderType, const std::wstring& path,  void* outInfo) = 0;
	virtual void RequestService(GraphicsService::SetRenderTarget renderWhere, const size_t width, const size_t height, void* outInfo) = 0;
	virtual void RequestService(GraphicsService::Render drawHow, void* outInfo) = 0;
};


