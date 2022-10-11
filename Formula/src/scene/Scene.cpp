#include "fm_pch.h"
#include "Scene.h"

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), std::make_shared<Scene>()};
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? "Entity" : name;
	
	return entity;
}

void Scene::OnUpdate()
{
}
