#include "fm_pch.h"
#include "GeometryGenerator.h"

MeshData
GeometryGenerator::CreateGrid(float width, float depth, UINT32 m, UINT32 n, std::string name)
{
	using VtxSize_t = std::vector<Vertex, std::allocator<Vertex>>::size_type;
	using IdxSize_t = std::vector<UINT32, std::allocator<uint32_t>>::size_type;

	MeshData meshData(name);

	size_t vertexCount = m * n;

	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (n - 1);
	float dz = depth / (m - 1);

	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	meshData.Vertices.resize(vertexCount);

	for (UINT32 i = 0; i < m; ++i)
	{
		float z = halfDepth - i * dz;

		for (UINT32 j = 0; j < n; ++j)
		{
			float x = -halfWidth + j * dx;

			meshData.Vertices[(size_t)(i * n) + j].Position = FMVec3(x, 0.0f, z);
			meshData.Vertices[(size_t)(i * n) + j].Normal	= FMVec3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[(size_t)(i * n) + j].TangentU = FMVec3(1.0f, 0.0f, 0.0f);
			
			meshData.Vertices[(size_t)(i * n) + j].TexC.x = j * du;
			meshData.Vertices[(size_t)(i * n) + j].TexC.y = i * dv;
		}
	}

	size_t indexCount = m * (2 * (n - 1)) + n * (2 * (m - 1));
	
	meshData.Indices32.resize(indexCount);
	
	// This is for large meshes with more than 64K vertices, otherwise we fall back to 16-bit indices.
	UINT32 k = 0;
	for (UINT32 i = 0; i < m; ++i)
	{
		for (UINT32 j = 0; j < n - 1; ++j)
		{
			meshData.Indices32[k]		= (i * n) + j;
			meshData.Indices32[(size_t)k + 1]	= (i * n) + j + 1;
			
			k += 2;
		}
	}

	for (UINT32 i = 0; i < m - 1; ++i)
	{
		for (UINT32 j = 0; j < n; ++j)
		{
			meshData.Indices32[k] = (i * n) + j;
			meshData.Indices32[(size_t)k + 1] = ((i + 1) * n) + j;

			k += 2;
		}
	}

	return meshData;
}
