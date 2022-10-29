#pragma once

//TODO: Let users interface with the advanced graphic features per API via GUI.

// Grahpics API basic functionalities
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

	enum class Begin : UINT8
	{
		BEGIN_SCENEFRAME,
	};

	enum class PreProcess : UINT8
	{
		GRAPHICS_PIPELINE,
		COMPUTE_PIPELINE,
	};

	enum class UploadStaticResource : UINT8
	{
		MESH,
		TEXTURE,

		MAIN_PASS,
		MAIN_PROP,

		UNLIT_PASS,
		UNLIT_PROP,
	};

	enum class AllocateDynamicResource : UINT8
	{
		MESH,
		TEXTURE,

		MAIN_PASS,
		MAIN_PROP,

		UNLIT_PASS,
		UNLIT_PROP,
	};

	enum class UploadDynamicResource : UINT8
	{
		MESH,
		TEXTURE,

		MAIN_PASS,
		MAIN_PROP,

		UNLIT_PASS,
		UNLIT_PROP,
	};

	enum class SetRenderer : UINT8
	{
		MESH_RENDERER,
		
		LINE_RENDERER,
		
		PARTICLE_RENDERER,
	};

	enum class SetViewPort : UINT8
	{
		EDITOR,
		
		GAME,

		AUX0,
		AUX1,
		AUX2,
		AUX3,
		AUX4,
	};

	enum class Enqueue : UINT8
	{
		DRAW_CALL,
	};

	enum class End : UINT8
	{
		END_SCENEFRAME,
	};

};

class Renderer
{
public:
	virtual ~Renderer() {}

	// Graphics API Overloads
	virtual void RequestService(GraphicsService::Begin what, const void* _opt_in_Info, void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::PreProcess what, const void* _opt_in_Info, void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::UploadStaticResource what, const std::wstring& path, const void* _opt_in_Info,  void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::AllocateDynamicResource what, const void* _opt_in_Info,  void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::UploadDynamicResource what, const void* _opt_in_Info,  void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::SetRenderer what, const void* _opt_in_Info,  void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::SetViewPort what, const int width, const int height, const void* _opt_in_Info, void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::Enqueue what, const void* _opt_in_Info, void* _opt_out_Info) = 0;
	virtual void RequestService(GraphicsService::End what, const void* _opt_in_Info, void* _opt_out_Info) = 0;
};


