#include "fm_pch.h"
#include "DX12Renderer.h"

#include "core/d3dx12_rscalloc.h"

void DirectX12Renderer::RequestService(GraphicsService::AllocateGPUMemory allocWhat, const void* initData, SafelyCopyablePointer<void> outInfo)
{
	if (allocWhat == GraphicsService::AllocateGPUMemory::MESH)
	{
		FM_ASSERT(initData != nullptr);

		MeshBufferView* meshHandle = new MeshBufferView();

		MeshData* mesh = (MeshData*)initData;
		
		auto vertexByteStride = sizeof(Vertex);
		auto vertexCount = mesh->Vertices.size();
		auto verticesByteSize = vertexByteStride * vertexCount;

		auto indexCount =  mesh->Indices32.size();
		auto IndicesByteSize = mesh->isIndices32 ? sizeof(UINT32) * indexCount : sizeof(UINT16) * indexCount;

		ComPtr<ID3D12Resource> verticesResource = DefaultBufferAllocator(g_pd3dDevice, mesh->Vertices.data(), verticesByteSize);
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

		SafelyCopyablePointer<MeshBufferView> safeMeshHandle(meshHandle);
		outInfo = safeMeshHandle;
	}

	else if (allocWhat == GraphicsService::AllocateGPUMemory::CONSTANT)
	{
		
	}

	else
	{

	}
}
