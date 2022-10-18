#pragma once



// Grahics Device Basic Functionalities
struct GraphicsService
{
public:
	static enum class GrpahicsAPI : UINT8
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

	static enum class AllocateGPUMemory : UINT8
	{
		MESH,
		CONSTANT,
		TEXTURE,
	};

	static enum class BindShaderProgram : UINT8
	{
		GRAPHICS,
		COMPUTE,
	};
	
	static enum class Draw
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
	virtual void RequestService(GraphicsService::AllocateGPUMemory allocWhat, _In_ const void* initData, _Out_ SafelyCopyablePointer<void> outInfo) = 0;
	virtual void RequestService(GraphicsService::BindShaderProgram usage, const std::wstring& path, _Out_ SafelyCopyablePointer<void> outInfo) = 0;
	virtual void RequestService(GraphicsService::Draw drawMode) = 0;
};


