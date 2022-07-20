#pragma once
#include "Renderer.h"

namespace Pearl
{	
	// Forward Declarations
	// - Note: This is only temporary; they are going to be replaced later.
	class GameObject;
	typedef unsigned int RenderProgramID;

	class RenderManager 
	{
	public:
		RenderManager() {}
		~RenderManager() {}

		//---------------------------------------------------------------
		// Application's High-Level View of Rendering Pipeline
		//---------------------------------------------------------------
		
		static void StartUp();
		static void CreateRenderProgram(const GameObject& gameObject);
		
		//TODO: Use the scene graph to organize the game objects by its rendering program id before setting this program to use.
		static void UseRenderProgram(RenderProgramID id);  

		static void BindResources(const GameObject& gameObject);
		static void Render();
		static void ShutDown();

		//---------------------------------------------------------------
		//---------------------------------------------------------------
	
	private:
		// Service API
		static std::unique_ptr<Renderer> Renderer_; 
	};
}