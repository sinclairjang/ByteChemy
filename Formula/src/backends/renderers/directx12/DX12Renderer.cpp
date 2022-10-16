#include "fm_pch.h"
#include "DX12Renderer.h"

#include "core/d3dx12_rscalloc.h"
#include "core/d3dx12_rootsigner.h"

void DirectX12Renderer::RequestService(GraphicsService::AllocateGPUMemory allocWhat, SafelyCopyablePointer<const void> initData, SafelyCopyablePointer<void> outInfo)
{
	if (allocWhat == GraphicsService::AllocateGPUMemory::MESH)
	{
		FM_ASSERT(initData != nullptr);

		
		MeshData* mesh = std::static_pointer_cast<MeshData>(initData).get();
		
		auto vertexByteStride = sizeof(Vertex);
		auto vertexCount = mesh->Vertices.size();
		auto verticesByteSize = vertexByteStride * vertexCount;

		auto indexCount =  mesh->Indices32.size();
		auto IndicesByteSize = mesh->isIndices32 ? sizeof(UINT32) * indexCount : sizeof(UINT16) * indexCount;

		ComPtr<ID3D12Resource> verticesResource = DefaultBufferAllocator(g_pd3dDevice, mesh->Vertices.data(), verticesByteSize);

		MeshBufferView* meshHandle = new MeshBufferView();
		meshHandle->vbv.BufferLocation = verticesResource->GetGPUVirtualAddress();
		meshHandle->vbv.StrideInBytes = vertexByteStride;
		meshHandle->vbv.SizeInBytes = verticesByteSize;

		if (mesh->isIndices32)
		{
			ComPtr<ID3D12Resource> indicesResource = DefaultBufferAllocator(g_pd3dDevice, mesh->Indices32.data(), IndicesByteSize);
			meshHandle->ibv.BufferLocation = indicesResource->GetGPUVirtualAddress();
			meshHandle->ibv.Format = DXGI_FORMAT_R32_UINT;
			meshHandle->ibv.SizeInBytes = IndicesByteSize;
		}
		else
		{
			ComPtr<ID3D12Resource> indicesResource = DefaultBufferAllocator(g_pd3dDevice, mesh->GetIndices16().data(), IndicesByteSize);
			meshHandle->ibv.BufferLocation = indicesResource->GetGPUVirtualAddress();
			meshHandle->ibv.Format = DXGI_FORMAT_R16_UINT;
			meshHandle->ibv.SizeInBytes = IndicesByteSize;
		}

		SafelyCopyablePointer<void> safeMeshHandle(meshHandle);
		outInfo = safeMeshHandle;
	}

	else if (allocWhat == GraphicsService::AllocateGPUMemory::CONSTANT)
	{
		
	}

	else
	{

	}
}

void DirectX12Renderer::RequestService(GraphicsService::BindShaderProgram bindHow, const std::wstring& path, _Out_ SafelyCopyablePointer<void> outInfo)
{
	namespace fs = std::filesystem;

	//TODO: Later we dynamically extract graphics pipeline setup informations from custom format, namely a file with extension '.formula'.
	// For the time being, we code by hand the graphics render pipeline based on hlsl files.
 
	// Cache
	std::string filename = ws2s(fs::path(path).filename());
	std::string stem = ws2s(fs::path(path).stem());
	std::string ext = ws2s(fs::path(path).extension());

	//FM_ASSERTM(ext == "formula", "Requested shader file format is not supported.");

	if (bindHow == GraphicsService::BindShaderProgram::GRAPHICS)
	{
		if (filename == "Color.hlsl") 
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
		


		}
	}


}
