#pragma once

#include "Entity.h"

class Scene
{
	friend Entity;

public:
	Entity CreateEntity(const std::string& name = std::string());

	void OnUpdate();

private:
	entt::registry m_Registry;
};

