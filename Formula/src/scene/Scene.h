#pragma once

#include "Entity.h"

class Scene
{
	friend Entity;

public:
	Entity CreateEntity(const std::string& name = std::string());
	void LoadMeshAsset();
	void LoadShaderAsset(); // Which in turn loads the relevant root signature and resource buffers.
	void OnCreate();
	void OnUpdate();

private:
	entt::registry m_Registry;
};

