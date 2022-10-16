#include "fm_pch.h"
#include "Scene.h"

#include "Component.h"
#include "core/ServiceLocator.h"
#include "core/GeometryGenerator.h"
#include "backends/renderers/directx12/DX12Renderer.h"		// #include "backends/renderers/Vulkan/VKRenderer.h"

Scene::Scene()
{
	m_SceneEntity = m_Registry.create();
	m_Registry.emplace<GraphicsComponent>(m_SceneEntity);
}

Entity Scene::CreateEntity(const std::string& name)
{
	Entity entity = { m_Registry.create(), std::make_shared<Scene>()};
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? L"Entity" : s2ws(name);
	return entity;
}

void Scene::SetRenderer(const GraphicsService::GrpahicsAPI graphicsAPI)
{
	if (graphicsAPI == GraphicsService::GrpahicsAPI::DirectX12)
	{
		ServiceLocator<Renderer>::Provide(&DX12Renderer);
		m_Renderer = ServiceLocator<Renderer>::GetService();

		auto& graphic = m_Registry.get<GraphicsComponent>(m_SceneEntity);
		graphic.GraphicsAPI = s2ws("DirectX12");
	}

	else
	{
		FM_ASSERTM(0, "Requested Graphics API is not yet supported.");
	}
}

void Scene::LoadMeshAsset(const std::wstring& path)
{
	auto& gridMesh = m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateGrid(10, 10, 100, 100)), "Grid");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateCube(...)), "Cube");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateCylinder(...)), "Cylinder");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateSphere(...)), "Sphere");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateGeoSphere(...)), "GeoSphere");
	//m_Registry.emplace<MeshFilterComponent>(m_SceneEntity, std::move(GeometryGenerator::CreateFBXGeometryFromFile(...)), "FBXMesh");

	SafelyCopyablePointer<const void>	cp_GridMesh(&gridMesh);
	SafelyCopyablePointer<void>			cp_MeshHandle;
	m_Renderer->RequestService(GraphicsService::AllocateGPUMemory::MESH, cp_GridMesh, cp_MeshHandle);

	auto& graphic = m_Registry.get<GraphicsComponent>(m_SceneEntity);
	graphic.MeshHandle = cp_MeshHandle.get();
}

void Scene::LoadShaderAsset(const std::wstring& path)
{
	SafelyCopyablePointer<void> cp_GraphicsPipelineHandle;
	m_Renderer->RequestService(GraphicsService::BindShaderProgram::GRAPHICS, path, cp_GraphicsPipelineHandle);

	auto& graphic = m_Registry.get<GraphicsComponent>(m_SceneEntity);
	graphic.GPUGraphicsPipelineHandle = cp_GraphicsPipelineHandle.get();
}

