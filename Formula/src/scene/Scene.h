#pragma once

#include "renderer/Renderer.h"
#include "scene/CameraController.h"

class Entity;

// Application's basic program flow is	1) ImGui polls user events
//										2) ImGui provides graphical controller interface to Scene responsible for managing its game objects (i.e. entities) via entt library
//										3) Scene in turn requests appropriate tasks from its graphics backend
//										4) ImGui creates window(s) containing Scene's viewports among others
//										4) Jump to step 1) and repeat

class Scene
{
	friend class Entity;
	friend class EditorMainCamera;

public:
	Scene() = default;

	//TODO: Ensure an old scene will have been deleted by the time a new Scene instance is deserialized
	~Scene() = default;

	void Init();

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
	void LoadEngineShaderAssets();
	void LoadEngineMeshAssets();
	void LoadEngineTexImageAssets();

	void AllocatePassBuffers();

	//TODO:
	// Load external assets 1) after deserialization
	//						2) during runtime
	//void LoadMeshAssetFromFile(const std::wstring& path);
	//void LoadShaderAssetFromFile(const std::wstring& path);
	//void LoadTexImageAssetFromFile(const std::wstring& path);

	//TODO: De-/serialize assets to save/load them

public:
	// Register the entity to the scene
	Entity CreateEntity(const std::wstring& name);

	template<typename Component>
	void OnCreateEntity(void freeFunc(entt::registry&, entt::entity))
	{
		m_Registry.on_construct<Component>().connect<freeFunc>;
	}
	
	template<typename Component>
	void OnUpdateEntity(void freeFunc(entt::registry&, entt::entity))
	{
		m_Registry.on_update<Component>().connect<freeFunc>;

	}

	template<typename Component>
	void OnDestroyEntity(void freeFunc(entt::registry&, entt::entity))
	{
		m_Registry.on_destroy<Component>().connect<freeFunc>;

	}

public:
	// Locate graphics API
	void SetRenderAPI(const GraphicsService::GrpahicsAPI& graphicsAPI);
	void* GetTexID() const { return m_TexID; }

	void ParseAndProcessResponse(void*& _opt_out_info_);

private:
	entt::registry	m_Registry;

	// Scene Render Informations
	std::string m_GraphicsAPI;
	Renderer* m_Renderer;

	EditorMainCamera m_MainCamera;

	//TEMP
	void* m_TexID;
};