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
	// Scene timeline

	// Pre-process the scene buffer
	void Begin(const int width, const int height);

	// Upload resources updated via ImGui
	void Update();

	// Dispatch draw calls
	void Render();
	
	// Post-process the scene buffer
	void End();

public:
	// Load scene assets (e.g. mesh, material, shader etc.) 

	// Load built-in assets upon engine initialization 
	void LoadEngineMeshAssets();
	void LoadEngineShaderAssets();
	void LoadEngineTexImageAssets();

	// Load external assets 1) after deserialization
	//						2) during runtime
	void LoadMeshAssetFromFile(const std::wstring& path);
	void LoadShaderAssetFromFile(const std::wstring& path);
	void LoadTexImageAssetFromFile(const std::wstring& path);

	//TODO: De-/serialize assets to save/load them

public:
	// Register the entity to the scene
	Entity CreateEntity(const std::wstring& name);

	// Locate graphics API
	void SetRenderAPI(const GraphicsService::GrpahicsAPI& graphicsAPI);

private:
	entt::registry	m_Registry;

	// Scene Render Informations
	std::string m_GraphicsAPI;
	Renderer* m_Renderer;
};