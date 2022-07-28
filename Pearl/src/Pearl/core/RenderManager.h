#pragma once

#include "ServiceLocator.h"
#include "PlatformManager.h"
#include "Renderer.h"
#include "Option.h"

namespace Pearl
{	
	class RenderManager 
	{
		friend ServiceLocator<Renderer>;

	public:
		RenderManager();
		RenderManager(const RenderManager& rhs) = delete;
		RenderManager& operator=(const RenderManager& rhs) = delete;
		~RenderManager();

		//---------------------------------------------------------------
		// Application's High-Level View of Rendering Pipeline
		//---------------------------------------------------------------
		
		void StartUp(const PlatformManager& platform); // Initialize device and its rendering context based on a chosen platform
		void CreateRenderProgram(const std::fstream& shaderFile); // Graphics pipeline state setup using shader file
		
		//TODO: Use the scene graph to organize the game objects by its rendering program id before setting this program to use.
		void UseRenderProgram(RenderProgramID id); // Graphics pipeline state referenced by RenderProgramID

		void BindResources(const GameObject& gameObject); // Mesh, texture, etc per GameObject that have been loaded from Scene 
		void Render(); // Submit draw command
		void ShutDown(); // Clean up device and its rendering context

		//---------------------------------------------------------------
		//---------------------------------------------------------------
	
	private:
		std::unique_ptr<Renderer> Renderer_; // Service API to be set by ServiceLocator
		Option renderOption_;
	};
}
