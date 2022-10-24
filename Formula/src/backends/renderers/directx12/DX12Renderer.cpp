#include "fm_pch.h"
#include "DX12Renderer.h"

#include "core/DX12App_ErrorHandler.h"
#include "core/DX12App_ResourceAllocator.h"
#include "core/DX12App_RootSigner.h"
#include "core/DX12App_ShaderGenerator.h"
#include "core/DX12App_SceneBuffer.h"

// Import from ImGui main framework
extern ID3D12Device* g_pd3dDevice;

// Export
DX12Renderer g_dx12Renderer;


void DX12Renderer::RequestService(GraphicsService::AllocateGPUMemory allocWhat, const std::wstring& path, const void* initData, void* outInfo)
{
	if (allocWhat == GraphicsService::AllocateGPUMemory::MESH)
	{
		if (initData != nullptr);
		{
			return;
		}

		MeshData* pMesh = (MeshData*)initData;

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

	else if (allocWhat == GraphicsService::AllocateGPUMemory::CONSTANT)
	{
		
	}

	else  // == GraphicsService::AllocateGPUMemory::TEXTURE
	{
		namespace fs = std::filesystem;

		// Cache
		std::wstring filename = fs::path(path).filename();
		std::wstring stem = fs::path(path).stem();
		std::wstring ext = fs::path(path).extension();

		std::unique_ptr<ImageTexture> imageTex = std::make_unique<ImageTexture>(g_pd3dDevice);

		imageTex->CreateImageTextureFromFile(path);

		m_ImageTex[ws2s(stem)] = std::move(imageTex);
	}
}

void DX12Renderer::RequestService(GraphicsService::CreateGPUProgram shaderType, const std::wstring& path, void* outInfo)
{
	namespace fs = std::filesystem;

	// Cache
	std::wstring filename = fs::path(path).filename();
	std::wstring stem = fs::path(path).stem();
	std::wstring ext = fs::path(path).extension();

	//FM_ASSERTM(ext == "formula", "Requested shader file format is not supported.");

	if (shaderType == GraphicsService::CreateGPUProgram::GRAPHICS)
	{
		//TODO: Later we procedurally extract resource binding informations from custom format dynamically, namely a file with extension '.formula'.
		// For the time being, we code by hand the graphics render pipeline based on hlsl files.

		if (filename == L"Color.hlsl")  // Built-in shader
		{
			m_RootSignature = CreateScope<RootSignature>(g_pd3dDevice);
			
			std::vector<RootParmeter> rootParams
			{
				//TEMP
				RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b0)
				RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b1)
				RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b2)
				RootParmeter(LeafParametersLayout::TABLE,	{ LeafParameterArray(LeafParameterType::CBV, 1) }),  // -> register (b3)
			};

			m_RootSignature->CreateGraphicsRootSignature(rootParams);
		
			Shader shaderProgram(g_pd3dDevice, m_RootSignature.get());
			
			//TODO: Let users interface with the advanced graphic features per API via GUI.
			// For the time being, we code by hand those features given a shader file.

			auto& pipeSpec = GPUPipelineSpecification::Primitive(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			shaderProgram.CreateGraphicsShader(path, pipeSpec);

		
		}
		
		else  // == GraphicsService::BindShaderProgram::COMPUTE
		{
			FM_ASSERTM(0, "Unknown shader file");
		}
	}
	else
	{
		FM_ASSERTM(0, "Requested service(Compute Shader) is not yet supported");
	}
}

void DX12Renderer::RequestService(GraphicsService::SetRenderTarget renderWhere, const size_t width, const size_t height, void* outInfo)
{
	if (renderWhere == GraphicsService::SetRenderTarget::FRAMEBUFFER)
	{
		FM_ASSERTM(0, "Requested Service(Frame Buffer) is not yet supported");
	}
	else  // == GraphicsService::SetRenderTarget::TEXTURE
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
}
