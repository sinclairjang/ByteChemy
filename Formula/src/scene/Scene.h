#pragma once

#include "Entity.h"

class Renderer;
class TimeStep;

class Scene
{
	friend Entity;

	Scene();
	~Scene(); //TODO: Ensure an old scene will have been deleted by the time a new instance is deserialized

public:
	Entity CreateEntity(const std::string& name = std::string());

	void SetRenderer(const GraphicsService::GrpahicsAPI graphicsAPI);

	void LoadMeshAsset(const std::wstring& path);
	void LoadShaderAsset(const std::wstring& path);

	void OnUpdate(TimeStep ts);

private:
	entt::registry	m_Registry;

	// Scene Render Informations
	Renderer*	m_Renderer;
	std::wstring m_GraphicsAPI;

	std::vector<MeshData> m_MeshData;

	// Driver-specific opaque data 
	// These may outlast the associated scene as long as the new scene still uses those GPU resources.
	HashTable<std::wstring, SafelyCopyablePointer<void>> h_MeshIndex;
	HashTable<std::wstring, SafelyCopyablePointer<void>> h_ShaderIndex;
};
