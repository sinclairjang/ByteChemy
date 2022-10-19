#include "fm_pch.h"
#include "DX12Renderer.h"

#include "core/d3dx12_rscalloc.h"
#include "core/d3dx12_rootsigner.h"
#include "core/d3dx12_shadergen.h"

// Import
extern ID3D12Device* g_pd3dDevice;

// Export
DirectX12Renderer DX12Renderer;

void DirectX12Renderer::RequestService(GraphicsService::AllocateGPUMemory allocWhat, const void* initData, SafelyCopyablePointer<void> outInfo)
{
	if (allocWhat == GraphicsService::AllocateGPUMemory::MESH)
	{
		FM_ASSERT(initData != nullptr);

		MeshData* pMesh = (MeshData*)initData;
		
		auto vertexByteStride = sizeof(Vertex);
		auto vertexCount = pMesh->Vertices.size();
		auto verticesByteSize = vertexByteStride * vertexCount;

		auto indexCount = pMesh->Indices32.size();
		auto IndicesByteSize = pMesh->isIndices32 ? sizeof(UINT32) * indexCount : sizeof(UINT16) * indexCount;

		ComPtr<ID3D12Resource> verticesResource = DefaultBufferAllocator(g_pd3dDevice, pMesh->Vertices.data(), verticesByteSize);

		MeshBufferView* pMeshHandle = new MeshBufferView();
		pMeshHandle->vbv.BufferLocation = verticesResource->GetGPUVirtualAddress();
		pMeshHandle->vbv.StrideInBytes = (UINT)vertexByteStride;
		pMeshHandle->vbv.SizeInBytes = (UINT)verticesByteSize;

		if (pMesh->isIndices32)
		{
			ComPtr<ID3D12Resource> indicesResource = DefaultBufferAllocator(g_pd3dDevice, pMesh->Indices32.data(), IndicesByteSize);
			pMeshHandle->ibv.BufferLocation = indicesResource->GetGPUVirtualAddress();
			pMeshHandle->ibv.Format = DXGI_FORMAT_R32_UINT;
			pMeshHandle->ibv.SizeInBytes = (UINT)IndicesByteSize;
		}
		else
		{
			ComPtr<ID3D12Resource> indicesResource = DefaultBufferAllocator(g_pd3dDevice, pMesh->GetIndices16().data(), IndicesByteSize);
			pMeshHandle->ibv.BufferLocation = indicesResource->GetGPUVirtualAddress();
			pMeshHandle->ibv.Format = DXGI_FORMAT_R16_UINT;
			pMeshHandle->ibv.SizeInBytes = (UINT)IndicesByteSize;
		}

		SafelyCopyablePointer<void> spMeshHandle(pMeshHandle);
		outInfo = spMeshHandle;
	}

	else if (allocWhat == GraphicsService::AllocateGPUMemory::CONSTANT)
	{
		
	}

	else
	{

	}
}

void DirectX12Renderer::RequestService(GraphicsService::BindShaderProgram bindHow, const std::wstring& path,  SafelyCopyablePointer<void> outInfo)
{
	namespace fs = std::filesystem;

	// Cache
	std::wstring filename = fs::path(path).filename();
	std::wstring stem = fs::path(path).stem();
	std::wstring ext = fs::path(path).extension();

	//FM_ASSERTM(ext == "formula", "Requested shader file format is not supported.");

	if (bindHow == GraphicsService::BindShaderProgram::GRAPHICS)
	{
		//TODO: Later we procedurally extract resource binding informations from custom format dynamically, namely a file with extension '.formula'.
		// For the time being, we code by hand the graphics render pipeline based on hlsl files.

		if (filename == L"Color.hlsl") 
		{
			if (!m_RootSigTable.contains("ROOT:DC1,DC1,DC1"))
			{
				RootSignature ROOTSIG_DC1_DC1_DC1(g_pd3dDevice);
				std::vector<RootParmeter> rootParams
				{
					RootParmeter(LeafParametersLayout::DESCRIPTOR, { LeafParameterArray(LeafParameterType::CBV, 1) }),
					RootParmeter(LeafParametersLayout::DESCRIPTOR, { LeafParameterArray(LeafParameterType::CBV, 1) }),
					RootParmeter(LeafParametersLayout::DESCRIPTOR, { LeafParameterArray(LeafParameterType::CBV, 1) })
				};

				ROOTSIG_DC1_DC1_DC1.CreateGraphicsRootSignature(rootParams);

				m_RootSigTable.insert({ "ROOT:DC1,DC1,DC1", ROOTSIG_DC1_DC1_DC1 });
			}
		
			auto search = m_RootSigTable.find("ROOT:DC1,DC1,DC1");
			Shader shaderProgram(g_pd3dDevice, search->second);
			
			//TODO: Let users interface with the advanced graphic features per API via GUI.
			// For the time being, we code by hand those features given a shader file.

			auto& pipeSpec = GPUPipelineSpecification::Primitive(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			shaderProgram.CreateGraphicsShader(path, pipeSpec);

			SafelyCopyablePointer<void> spGraphicsPipelineHandle(shaderProgram.GetGraphicsPipelineHandle(), 
				[](ID3D12PipelineState* ps) { ps->Release(); });
			
			outInfo = spGraphicsPipelineHandle;
		}
		
		else
		{
			FM_ASSERTM(0, "Unknown shader file");
		}
	}
	else
	{
		FM_ASSERTM(0, "Requested compute shader is not yet supported");
	}
}
