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

	enum class BindShaderProgram : UINT8
	{
		GRAPHICS,
		COMPUTE,
	};
	
	enum class SetRenderTarget : UINT8
	{
		FRAMEBUFFER,
		TEXTURE,
	};

	enum class DispatchDrawCall : UINT8
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
	virtual void RequestService(GraphicsService::AllocateGPUMemory type, const void* initData,  SafelyCopyablePointer<void> outInfo) = 0;
	virtual void RequestService(GraphicsService::BindShaderProgram type, const std::wstring& path,  SafelyCopyablePointer<void> outInfo) = 0;
	virtual void RequestService(GraphicsService::SetRenderTarget type, const size_t width, const size_t height, SafelyCopyablePointer<void> outInfo) = 0;
	//virtual void RequestService(GraphicsService::Draw drawMode) = 0;
};


