#include "fm_pch.h"
#include "Scene.h"

#include "Component.h"
#include "core/ServiceLocator.h"
#include "core/GeometryGenerator.h"
#include "backends/renderers/directx12/DX12Renderer.h"

// Import
//extern DirectX12Renderer DX12Renderer;

//void DX12RendererUsage()
//{
//	ServiceLocator<Renderer>::Provide(&DX12Renderer);
//	Renderer* renderer = ServiceLocator<Renderer>::GetService();
//	renderer->...;
//}

Scene::Scene()
{
	m_SceneEntity = m_Registry.create();
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), std::make_shared<Scene>()};
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? L"Entity" : s2ws(name);
	return entity;
}

void Scene::LoadMeshAsset(Renderer* renderer, const std::string& path)
{
	auto& gridMesh = m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateGrid(10, 10, 100, 100)), "Grid");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateCube(...)), "Cube");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateCylinder(...)), "Cylinder");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateSphere(...)), "Sphere");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateGeoSphere(...)), "GeoSphere");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateFBXGeometryFromFile(...)), "FBXMesh");

	renderer->AllocateGPUResource(&gridMesh.Mesh);
}

void Scene::LoadShaderAsset(Renderer* renderer, const std::string& path)
{

}

