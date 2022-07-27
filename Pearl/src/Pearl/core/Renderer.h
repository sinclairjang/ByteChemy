#pragma once

namespace Pearl
{
	// Forward Declarations
	// - Note: This is only temporary; they are going to be replaced later.
	class GameObject;
	using RenderProgramID = unsigned int;
	
	class Renderer
	{
	public:
		virtual ~Renderer();

		virtual void StartUp();
		virtual void CreateRenderProgram(const std::fstream& shaderFile);
		virtual void UseRenderProgram(RenderProgramID id);
		virtual void BindResources(const GameObject& gameObject);
		virtual void Render();
		virtual void ShutDown();
	};

}