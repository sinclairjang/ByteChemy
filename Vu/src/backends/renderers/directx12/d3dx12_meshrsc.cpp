#include "Vu_pch.h"
#include "d3dx12_meshrsc.h"

D3D12_VERTEX_BUFFER_VIEW MeshResource::VertexBufferView() const
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
	vbv.StrideInBytes = VertexByteStride;
	vbv.SizeInBytes = VertexBufferByteSize;

	return vbv;
}

D3D12_INDEX_BUFFER_VIEW MeshResource::IndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;

	return ibv;
}

void MeshResource::ReleaseUploaders()
{
	VertexBufferUploader = nullptr;
	IndexBufferUploader = nullptr;
}

void MeshResourceManager::Bind(MeshResource meshResource, MeshData meshData)
{

}
