#pragma once

#include "renderer/Renderer.h"

class Entity;

class Scene
{
	friend class Entity;

	Scene() = default;
	
	//TODO: Ensure an old scene will have been deleted by the time a new Scene instance is deserialized
	//~Scene(); 

public:
	// Initialize the scene
	void SetRenderer(const GraphicsService::GrpahicsAPI& graphicsAPI);
	void LoadMeshAsset(const std::string& name);
	void LoadMeshAssetFromFile(const std::wstring& path);
	void LoadShaderAssetFromFile(const std::wstring& path);
	
	// Pre-process the scene buffer
	void Begin(const size_t width, const size_t height);

	// Upload resources updated via ImGui
	void Update();

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
	std::weak_ptr<Renderer> m_Renderer;
	std::wstring	m_GraphicsAPI;
};