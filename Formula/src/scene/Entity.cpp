#include "fm_pch.h"
#include "Entity.h"
#include "Scene.h"

Entity::Entity(entt::entity handle, std::shared_ptr<Scene> scene)
	: m_EntityHandle(handle), m_Scene(scene)
{
}	

