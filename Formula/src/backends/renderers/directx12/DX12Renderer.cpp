#include "fm_pch.h"
#include "DX12Renderer.h"

#include "core/DX12App_ErrorHandler.h"
#include "core/DX12App_ResourceAllocator.h"
#include "core/DX12App_RootSigner.h"
#include "core/DX12App_PreProcessor.h"
#include "core/DX12App_SceneBuffer.h"

// Import from ImGui main framework
extern ID3D12Device* g_pd3dDevice;

// Export
DX12Renderer g_dx12Renderer;

namespace fs = std::filesystem;

void DX12Renderer::RequestService(GraphicsService::PreProcess what, const void* _opt_in_Info, void* _opt_out_Info)
{
	if (what == GraphicsService::PreProcess::GRAPHICS_PIPELINE)
	{
		//TODO: Add flag to determine the intended behavior
		// e.g. If _opt_in_Info was meant to be used to represent path, the right way to reflect that is as follows:
		//if (_opt_in_Info != nullptr && _opt_in_Info == PATH);
		//{
		//	std::wstring* path = (std::wstring*)_opt_in_Info;

		//	// Cache
		//	std::wstring filename = fs::path(*path).filename();
		//	std::wstring stem = fs::path(*path).stem();
		//	std::wstring ext = fs::path(*path).extension();
		//}

		m_RootSignature = CreateScope<RootSignature>(g_pd3dDevice);

		//Note: Every graphics pipeline will share the same shader resource binding scheme to increase the caching performance
		std::vector<RootParmeter> rootParams
		{
			//TEMP
			RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b0)
			RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b1)
			RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b2)
			RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b3)
		};

		m_RootSignature->CreateGraphicsRootSignature(rootParams);

		Plumber L_Unlit(g_pd3dDevice, m_RootSignature.get());
		auto& pipeSpec = GPUPipelineSpecification::Primitive(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
		L_Unlit.CreateGraphicsShader(
			L"..\\..\\..\\..\\assets\\shader\\unlit\\Unlit.hlsl",
			pipeSpec);
		m_MeshRendererPSOs.insert({ "MeshRenderer(UnlitShader)", L_Unlit.GetGraphicsPipelineHandle() });

		//Plumber M_Unlit(g_pd3dDevice, m_RootSignature.get());
		// ...

		//Plumber P_Unlit(g_pd3dDevice, m_RootSignature.get());
		// ...

		// ...
	}
	
	else
	{
		LOG_INFO("Only PreProcess(GRPHICS_PIPELINE) is supported for now");
	}



}

void DX12Renderer::RequestService(GraphicsService::LoadResource what, const std::wstring& path, const void* _opt_in_Info, void* _opt_out_Info)
{
	if (what == GraphicsService::LoadResource::MESH)
	{
		//TODO: Add flag to determine the intended behavior
		if (_opt_in_Info != nullptr);
		{
			return;
		}

		MeshData* pMesh = (MeshData*)_opt_in_Info;

		auto meshGeo = std::make_unique<MeshGeometry>();
		meshGeo->Name = pMesh->Name;
		
		//TODO: Extract submesh informations from MeshData to build DrawArgs programmatically
		//Temp 
		SubmeshGeometry subMeshGeo {};
		std::string subMeshName = meshGeo->Name;
		
		subMeshGeo.BaseVertexLocation = 0;
		subMeshGeo.StartIndexLocation = 0;
		subMeshGeo.IndexCount = 0;

		auto vertexCount = pMesh->Vertices.size();
		const UINT vbByteSize = sizeof(Vertex) * vertexCount;

		auto indexCount = pMesh->Indices32.size(); // == pMesh->GetIndices16().size()
		const UINT ibByteSize = pMesh->isIndices32 ? sizeof(UINT32) * indexCount : sizeof(UINT16) * indexCount;

		ThrowIfFailed(D3DCreateBlob(vbByteSize, &meshGeo->VertexBufferCPU));
		CopyMemory(meshGeo->VertexBufferCPU->GetBufferPointer(), pMesh->Vertices.data(), vbByteSize);

		meshGeo->VertexBufferGPU = DefaultBufferAllocator(g_pd3dDevice, pMesh->Vertices.data(), meshGeo->VertexBufferByteSize);

		if (pMesh->isIndices32)
		{
			ThrowIfFailed(D3DCreateBlob(ibByteSize, &meshGeo->IndexBufferCPU));
			CopyMemory(meshGeo->IndexBufferCPU->GetBufferPointer(), pMesh->Indices32.data(), ibByteSize);

			meshGeo->IndexBufferGPU = DefaultBufferAllocator(g_pd3dDevice, pMesh->Indices32.data(), ibByteSize);
		}
		else
		{
			ThrowIfFailed(D3DCreateBlob(ibByteSize, &meshGeo->IndexBufferCPU));
			CopyMemory(meshGeo->IndexBufferCPU->GetBufferPointer(), pMesh->GetIndices16().data(), ibByteSize);

			meshGeo->IndexBufferGPU = DefaultBufferAllocator(g_pd3dDevice, pMesh->GetIndices16().data(), ibByteSize);
		}

		meshGeo->VertexByteStride = sizeof(Vertex);
		meshGeo->VertexBufferByteSize = vbByteSize;
		meshGeo->IndexFormat = pMesh->isIndices32 ? DXGI_FORMAT_R32_UINT : DXGI_FORMAT_R16_UINT;
		meshGeo->IndexBufferByteSize = ibByteSize;

		//Temp
		meshGeo->DrawArgs[subMeshName] = subMeshGeo;

		m_MeshObjects[meshGeo->Name] = std::move(meshGeo);
	}

	else  // == GraphicsService::AllocateGPUMemory::TEXTURE
	{
		// Cache
		std::wstring filename = fs::path(path).filename();
		std::wstring stem = fs::path(path).stem();
		std::wstring ext = fs::path(path).extension();

		std::unique_ptr<ImageTexture> imageTex = std::make_unique<ImageTexture>(g_pd3dDevice);

		imageTex->CreateImageTextureFromFile(path);

		m_ImageTex[ws2s(stem)] = std::move(imageTex);
	}
}

void DX12Renderer::RequestService(GraphicsService::AllocateResource what, const void* _opt_in_Info, void* _opt_out_Info)
{
}

void DX12Renderer::RequestService(GraphicsService::SetRenderer what, const void* _opt_in_Info, void* _opt_out_Info)
{
}

void DX12Renderer::RequestService(GraphicsService::SetViewPort what, const int width, const int height, const void* _opt_in_Info, void* _opt_out_Info)
{
	if (what == GraphicsService::SetViewPort::EDITOR)
	{
		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
		dsvHeapDesc.NumDescriptors = 1;
		dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		dsvHeapDesc.NodeMask = 0;

		ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
			&dsvHeapDesc, IID_PPV_ARGS(m_DsvHeap.GetAddressOf())));

		m_DsvDescriptor = m_DsvHeap->GetCPUDescriptorHandleForHeapStart();

		D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
		srvHeapDesc.NumDescriptors = NUM_BACK_BUFFERS;
		srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
			&srvHeapDesc, IID_PPV_ARGS(m_SrvHeap.GetAddressOf())));

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		rtvHeapDesc.NumDescriptors = NUM_BACK_BUFFERS;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		rtvHeapDesc.NodeMask = 0;

		ThrowIfFailed(g_pd3dDevice->CreateDescriptorHeap(
			&rtvHeapDesc, IID_PPV_ARGS(m_RtvHeap.GetAddressOf())));

		SIZE_T rtvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RtvHeap->GetCPUDescriptorHandleForHeapStart();

		SIZE_T srvDescriptorSize = g_pd3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = m_SrvHeap->GetCPUDescriptorHandleForHeapStart();

		for (UINT i = 0; i < NUM_BACK_BUFFERS; i++)
		{
			auto sceneBuf = std::make_unique<RenderTexture>(DXGI_FORMAT_R8G8B8A8_UNORM);

			DirectX::XMVECTORF32 fColorValue = { 0.96f, 0.97f, 0.97f, 1.f };
			DirectX::XMVECTOR colorValue = fColorValue;
			sceneBuf->SetClearColor(colorValue);

			m_RtvDescriptors[i] = rtvHandle;
			rtvHandle.ptr += rtvDescriptorSize;

			m_SrvDescriptors[i] = srvHandle;
			srvHandle.ptr += srvDescriptorSize;

			sceneBuf->SetDevice(g_pd3dDevice, m_SrvDescriptors[i], m_RtvDescriptors[i]);

			sceneBuf->ResizeResource(width, height);

			m_SceneBuffers[i] = std::move(sceneBuf);
		}
	}
	else  // == GraphicsService::SetViewPort::GAME, AUXn
	{
		LOG_INFO("Only SetViewPort(Editor) is supported for now");
	}
}

void DX12Renderer::RequestService(GraphicsService::Enqueue what, const void* _opt_in_Info, void* _opt_out_Info)
{
}
