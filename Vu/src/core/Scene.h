#pragma once

#include "entt.hpp"

class Scene
{
	Entity CreateEntity();

	void OnUpdate();

private:
	entt::registry m_Registry;
};

