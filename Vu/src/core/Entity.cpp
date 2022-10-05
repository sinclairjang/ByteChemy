#include "Vu_pch.h"
#include "Entity.h"

Entity::Entity(entt::entity handle, std::shared_ptr<Scene> scene)
	: m_Scene(scene)
{
}	
