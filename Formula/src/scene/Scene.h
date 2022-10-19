#pragma once

#include "renderer/Renderer.h"

class Entity;
class TimeStep;

class Scene
{
	friend class Entity;

	Scene() = default;
	//~Scene(); //TODO: Ensure an old scene will have been deleted by the time a new instance is deserialized

public:
	// Initialize the scene
	void SetRenderer(const GraphicsService::GrpahicsAPI& graphicsAPI);
	void LoadMeshAsset(const std::wstring& path);
	void LoadShaderAsset(const std::wstring& path);
	
	// Pre-process the scene buffer
	void Begin();

	// Upload resources updated via ImGui
	void Update(TimeStep ts);

	// Dispatch draw calls
	void Render();
	
	// Post-process the scene buffer
	void End();

public:
	// Register the entity to the scene
	Entity CreateEntity(const std::string& name);



private:
	entt::registry	m_Registry;

	// Scene Render Informations
	Renderer*	m_Renderer;
	std::wstring m_GraphicsAPI;

	std::vector<MeshData> m_MeshData;

	// Driver-specific opaque data 
	// These may outlast the associated scene as long as the new scene still uses those GPU resources.
	HashTable<std::wstring, SafelyCopyablePointer<void>> h_MeshIndex;
	HashTable<std::wstring, SafelyCopyablePointer<void>> h_ShaderIndex;
};
