#include "fm_pch.h"
#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "core/ServiceLocator.h"
#include "core/GeometryGenerator.h"
#include "backends/renderers/directx12/DX12Renderer.h"		// #include "backends/renderers/Vulkan/VKRenderer.h"

void Scene::SetRenderAPI(const GraphicsService::GrpahicsAPI& graphicsAPI)
{
	if (graphicsAPI == GraphicsService::GrpahicsAPI::DirectX12)
	{
		Renderer* renderer = new DX12Renderer();
		ServiceLocator<Renderer>::Provide(renderer);
		m_Renderer = ServiceLocator<Renderer>::GetService();

		m_GraphicsAPI = "DirectX 12";
	}
}

void Scene::ParseAndProcessResponse(void*& _opt_out_info_)
{
	//TODO: As part of application-graphics hardware communication mechanism (optional)	
}

void Scene::LoadEngineShaderAssets()
{
	//void* info = nullptr;
	m_Renderer->RequestService(GraphicsService::PreProcess::GRAPHICS_PIPELINE, nullptr, nullptr);

	//ParseAndProcessResponse(info);
}

void Scene::LoadEngineMeshAssets()
{
	//TODO: Build and integrate with the geometry processing pipeline
	//Although, it is not so relevant now as we're setting up the program framework.
	//Simple geometries such as plane grid is sufficinet for now.
	
	MeshData& grid = GeometryGenerator::CreateGrid(20.0f, 30.0f, 60, 40);

	//void* info = nullptr;
	m_Renderer->RequestService(GraphicsService::UploadStaticResource::MESH, BUILT_IN, &grid, nullptr);
	
	//ParseAndProcessResponse(info);
}

void Scene::LoadEngineTexImageAssets()
{
	//void* info = nullptr;
	m_Renderer->RequestService(
		GraphicsService::UploadStaticResource::TEXTURE,
		L"assets\\texture\\White.jpeg", 
		nullptr, nullptr);

	//ParseAndProcessResponse(info);
}

void Scene::AllocatePassBuffers()
{
	{
		//void* info = nullptr;
		m_Renderer->RequestService(
			GraphicsService::AllocateDynamicResource::MAIN_PASS,
			nullptr, nullptr);

		//ParseAndProcessResponse(info);
	}

	{
		//void* info = nullptr;
		m_Renderer->RequestService(
			GraphicsService::AllocateDynamicResource::UNLIT_PASS,
			nullptr, nullptr);

		//ParseAndProcessResponse(info);
	}
}

void Scene::Init()
{
	LoadEngineShaderAssets();
	LoadEngineMeshAssets();
	LoadEngineTexImageAssets();

	AllocatePassBuffers();
}

void Scene::Begin(const int width, const int height)
{

	{
		void* info = nullptr;
		m_Renderer->RequestService(
			GraphicsService::Begin::BEGIN_SCENEFRAME,
			nullptr, info);

		//TEMP
		m_TexID = info;
		
		//ParseAndProcessResponse(info);
	}
	
	{
		//void* info = nullptr;
		m_Renderer->RequestService(
			GraphicsService::SetViewPort::EDITOR,
			width, height, nullptr, nullptr);

		//ParseAndProcessResponse(info);
	}


}

void Scene::Update()
{

}

void Scene::Render()
{
}

void Scene::End()
{
	delete m_Renderer;
}

Entity Scene::CreateEntity(const std::wstring& name)
{
	Entity entity = { m_Registry.create(), Ref<Scene>(this) };

	entity.AddComponent<TransformComponent>();
	auto& tag = entity.AddComponent<TagComponent>();
	
	tag.Tag = name.empty() ? L"Entity" : name;
	
	return entity;
}

