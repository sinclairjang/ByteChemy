#include "prlpch.h"
#include "RenderManager.h"

namespace Pearl
{
	RenderManager::RenderManager() 
		: Renderer_(), renderOption_()
	{
	}
	RenderManager::~RenderManager() 
	{
	}

	//---------------------------------------------------------------
	// Delegate to Underlying Renderer Service API(ex. Direct3D, OpenGL, Vulkan, Metal, etc)
	//---------------------------------------------------------------

	void RenderManager::StartUp(const PlatformManager& platform)
	{
		RenderManager::Renderer_->StartUp(platform);
	}

	void RenderManager::CreateRenderProgram(const std::fstream& shaderFile)
	{
		RenderManager::Renderer_->CreateRenderProgram(shaderFile);
	}

	void RenderManager::UseRenderProgram(RenderProgramID id)
	{
		RenderManager::Renderer_->UseRenderProgram(id);
	}

	void RenderManager::BindResources(const GameObject& gameObject)
	{
		RenderManager::Renderer_->BindResources(gameObject);
	}

	void RenderManager::Render()
	{
		RenderManager::Renderer_->Render();
	}

	void RenderManager::ShutDown()
	{
		RenderManager::Renderer_->ShutDown();
	}

	//---------------------------------------------------------------
	//---------------------------------------------------------------
}

