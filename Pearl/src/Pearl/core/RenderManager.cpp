#include "prlpch.h"
#include "RenderManager.h"

namespace Pearl
{
	RenderManager::RenderManager() 
	{
		// do nothing; merely in place to give precedence control
	}
	RenderManager::~RenderManager() 
	{
		// do nothing; merely in place to give precedence control
	}

	//---------------------------------------------------------------
	// Delegate to Underlying Renderer Service API(ex. Direct3D, OpenGL, Vulkan, Metal, etc)
	//---------------------------------------------------------------

	void RenderManager::StartUp()
	{
		RenderManager::Renderer_->StartUp();
	}

	void RenderManager::CreateRenderProgram(const GameObject& gameObject)
	{
		RenderManager::Renderer_->CreateRenderProgram(gameObject);
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

