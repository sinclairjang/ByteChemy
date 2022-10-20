#pragma once

#include "renderer/Renderer.h"

class Entity;
class TimeStep;

class Scene
{
	using MeshID = void;
	using ShaderID = void;
	using SceneBuffer = void;

	friend class Entity;

	Scene() = default;
	
	//TODO: Ensure an old scene will have been deleted by the time a new Scene instance is deserialized
	//~Scene(); 

public:
	// Initialize the scene
	void SetRenderer(const GraphicsService::GrpahicsAPI& graphicsAPI);
	void LoadMeshAsset(const std::wstring& path);
	void LoadShaderAsset(const std::wstring& path);
	
	// Pre-process the scene buffer
	void Begin(const size_t width, const size_t height);

	// Upload resources updated via ImGui
	void Update(TimeStep ts);

	// Dispatch draw calls
	void Render();
	
	// Post-process the scene buffer
	void End();

public:
	// Register the entity to the scene
	Entity CreateEntity(const std::wstring& name);



private:
	entt::registry	m_Registry;

	// Scene Render Informations
	Renderer*		m_Renderer;
	std::wstring	m_GraphicsAPI;

	std::vector<MeshData> m_MeshData;

	// Driver-specific opaque data 
	// These may outlast the associated scene as long as the new scene still uses those GPU resources.
	HashTable<std::wstring, SafelyCopyablePointer<MeshID>>		h_Mesh;				// Represents mesh data loaded in GPU
	HashTable<std::wstring, SafelyCopyablePointer<ShaderID>>	h_Shader;			// Represents shader programs compiled for GPU
	HashTable<std::wstring, SafelyCopyablePointer<SceneBuffer>> h_SceneView;		// Represents scene views
};
