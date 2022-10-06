#pragma once

#include "Scene.h"
#include "Component.h"

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, std::shared_ptr<Scene> scene);
	Entity(const Entity& other) = default;

	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		VU_ASSERT(!HasComponent<T>(), "Entity already has component!");
		T& component = m_Scene.get()->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);

		return component;
	}

	template<typename T>
	T& GetComponent()
	{
		VU_ASSERT(HasComponent<T>(), "Entity does not have component!");
		
		return m_Scene.get()->m_Registry.get<T>(m_EntityHandle);
	}

	template<typename T>
	bool HasComponent()
	{
		return m_Scene.get()->m_Registry.any_of<T>(m_EntityHandle);
	}

	template<typename T>
	void RemoveComponent()
	{
		VU_ASSERT(HasComponent<T>(), "Entity does not have component!");
		m_Scene.get()->m_Registry.remove<T>(m_EntityHandle);
	}

	operator bool() const { return m_EntityHandle != entt::null; }
	operator entt::entity() const { return m_EntityHandle; }
	operator UINT32() const{ return static_cast<UINT32>(m_EntityHandle); }
	
	bool operator==(const Entity& other) const
	{
		return (m_EntityHandle == other.m_EntityHandle) && (m_Scene.lock().get() == other.m_Scene.lock().get());
	}


	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

	const std::string& GetName() { return GetComponent<TagComponent>().Tag; }

private:
	entt::entity m_EntityHandle{ entt::null };
	std::weak_ptr<Scene> m_Scene;
};
