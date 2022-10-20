#include "fm_pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "core/ServiceLocator.h"
#include "core/GeometryGenerator.h"
#include "backends/renderers/directx12/DX12Renderer.h"		// #include "backends/renderers/Vulkan/VKRenderer.h"

// Import
extern DirectX12Renderer DX12Renderer;

void Scene::SetRenderer(const GraphicsService::GrpahicsAPI& graphicsAPI)
{
	if (graphicsAPI == GraphicsService::GrpahicsAPI::DirectX12)
	{
		ServiceLocator<Renderer>::Provide(&DX12Renderer);
		m_Renderer = ServiceLocator<Renderer>::GetService();

		m_GraphicsAPI = L"DirectX 12";
	}

	else
	{
		FM_ASSERTM(0, "Requested Graphics API is not yet supported");
	}
}

void Scene::LoadMeshAsset(const std::wstring& path)
{
	// Invoked on the start path for every mesh objects belonging to the scene

	//TODO: Integrate with the geometry processing pipeline
	// For the time being, we pilot the program with plane grid object.
	
	m_MeshData.push_back(GeometryGenerator::CreateGrid(10, 10, 100, 100));
	
	SafelyCopyablePointer<MeshID> spMesh;
	m_Renderer->RequestService(GraphicsService::AllocateGPUMemory::MESH, &m_MeshData[0], spMesh);

	h_Mesh.insert({ L"Grid", spMesh });
}

void Scene::LoadShaderAsset(const std::wstring& path)
{
	// Invoked on the start path for the predefined shader programs
	//TODO: Invoke on the update path for the custom shader programs

	SafelyCopyablePointer<ShaderID> spGraphicsShader;
	m_Renderer->RequestService(GraphicsService::BindShaderProgram::GRAPHICS, path, spGraphicsShader);

	h_Shader.insert({ path, spGraphicsShader });
}

void Scene::Begin(const size_t width, const size_t height)
{
	SafelyCopyablePointer<SceneBuffer> spSceneView;

	m_Renderer->RequestService(GraphicsService::SetRenderTarget::TEXTURE, width, height, spSceneView);

	h_SceneView.insert({ L"Main View", spSceneView });
}

//void Scene::Update(TimeStep ts)
//{
//}

void Scene::Render()
{

}

void Scene::End()
{

}

Entity Scene::CreateEntity(const std::wstring& name)
{
	// Invoked on the update path for a new game object

	Entity entity = { m_Registry.create(), SafelyCopyablePointer<Scene>(this) };
	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	tag.Tag = name.empty() ? L"Entity" : name;
	return entity;
}