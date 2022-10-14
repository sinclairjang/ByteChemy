#pragma once

#include "Entity.h"

class Scene
{
	friend Entity;
	
	Scene();
	//~Scene();

public:
	Entity CreateEntity(const std::string& name = std::string());
	
	void LoadMeshAsset(Renderer* renderer, const std::string& path = std::string());
	void LoadShaderAsset(Renderer* renderer, const std::string& path = std::string());
	
	//void OnUpdate(TimeStep ts);

private:
	entt::registry m_Registry;
	entt::entity m_SceneEntity{ entt::null };
};
