#pragma once

#include "Entity.h"

class Renderer;

class Scene
{
	friend Entity;
	
	Scene();
	~Scene();

public:
	Entity CreateEntity(const std::string& name = std::string());
	
	void SetRenderer(const GraphicsService::GrpahicsAPI graphicsAPI);

	void LoadMeshAsset(const std::string& path = std::string());
	void LoadShaderAsset(const std::string& path = std::string());
	
	//void OnUpdate(TimeStep ts);

private:
	entt::registry m_Registry;
	entt::entity m_SceneEntity{ entt::null };

	Renderer* m_Renderer;
};
