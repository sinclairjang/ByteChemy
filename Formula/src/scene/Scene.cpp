#include "fm_pch.h"
#include "Scene.h"

#include "Component.h"
#include "core/ServiceLocator.h"
#include "core/GeometryGenerator.h"
#include "backends/renderers/directx12/DX12Renderer.h"		// #include "backends/renderers/Vulkan/VKRenderer.h"

Entity Scene::CreateEntity(const std::string& name)
{
	// Invoked on the update path for a new game object

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
	
	SafelyCopyablePointer<void>	spMeshGPUIndex;
	m_Renderer->RequestService(GraphicsService::AllocateGPUMemory::MESH, &m_MeshData[0], spMeshGPUIndex);

	h_MeshIndex.insert({ L"Grid", spMeshGPUIndex });
}

void Scene::LoadShaderAsset(const std::wstring& path)
{
	// Invoked on the start path for the predefined shader programs
	//TODO: Invoke on the update path for the custom shader programs

	SafelyCopyablePointer<void> spGraphicsGPUProgramIndex;
	m_Renderer->RequestService(GraphicsService::BindShaderProgram::GRAPHICS, path, spGraphicsGPUProgramIndex);

	h_ShaderIndex.insert({ path, spGraphicsGPUProgramIndex });
}

