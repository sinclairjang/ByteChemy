#include "fm_pch.h"
#include "DX12Renderer.h"

#include "core/d3dx12_rscalloc.h"

void DirectX12Renderer::AllocateGPUResource(const void* initData)
{
	MeshData* mesh			= (MeshData*)initData;
	auto verticesByteSize	= sizeof(Vertex) * mesh->Vertices.size();
	auto IndicesByteSize	= sizeof(UINT32) * mesh->Indices32.size();

	ComPtr<ID3D12Resource> verticesResource = DefaultBufferAllocator(g_pd3dDevice, mesh->Vertices.data(), verticesByteSize);
	ComPtr<ID3D12Resource> indiceesResource = DefaultBufferAllocator(g_pd3dDevice, mesh->Indices32.data(), IndicesByteSize);
}
