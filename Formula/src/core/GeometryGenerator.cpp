#include "fm_pch.h"
#include "GeometryGenerator.h"

//TODO: Design a geometry pipeline

MeshData
GeometryGenerator::CreateCube(std::string name)
{
	//TEMP: Only use position data for testing
	MeshData meshData(name);

	meshData.Vertices.resize(8);

	meshData.Vertices[0].Position	= FMVec3(-1.0f, -1.0f, -1.0f);
	meshData.Vertices[0].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[0].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[0].TexC		= FMVec2(0.0f, 0.0f);

	meshData.Vertices[1].Position	= FMVec3(-1.0f, +1.0f, -1.0f);
	meshData.Vertices[1].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[1].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[1].TexC		= FMVec2(0.0f, 0.0f);
	
	meshData.Vertices[2].Position	= FMVec3(+1.0f, +1.0f, -1.0f);
	meshData.Vertices[2].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[2].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[2].TexC		= FMVec2(0.0f, 0.0f);
	
	meshData.Vertices[3].Position	= FMVec3(+1.0f, -1.0f, -1.0f);
	meshData.Vertices[3].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[3].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[3].TexC		= FMVec2(0.0f, 0.0f);
	
	meshData.Vertices[4].Position	= FMVec3(-1.0f, -1.0f, +1.0f);
	meshData.Vertices[4].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[4].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[4].TexC		= FMVec2(0.0f, 0.0f);
	
	meshData.Vertices[5].Position	= FMVec3(-1.0f, +1.0f, +1.0f);
	meshData.Vertices[5].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[5].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[5].TexC		= FMVec2(0.0f, 0.0f);
	
	meshData.Vertices[6].Position	= FMVec3(+1.0f, +1.0f, +1.0f);
	meshData.Vertices[6].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[6].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[6].TexC		= FMVec2(0.0f, 0.0f);
	
	meshData.Vertices[7].Position	= FMVec3(+1.0f, -1.0f, +1.0f);
	meshData.Vertices[7].Normal		= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[7].TangentU	= FMVec3(0.0f, 0.0f, 0.0f);
	meshData.Vertices[7].TexC		= FMVec2(0.0f, 0.0f);

	meshData.Indices32.resize(36);

	// Front face
	meshData.Indices32[0]	= 0;
	meshData.Indices32[1]	= 1;
	meshData.Indices32[2]	= 2;
	meshData.Indices32[3]	= 0;
	meshData.Indices32[4]	= 2;
	meshData.Indices32[5]	= 3;

	// Back face
	meshData.Indices32[6]	= 4;
	meshData.Indices32[7]	= 6;
	meshData.Indices32[8]	= 5;
	meshData.Indices32[9]	= 4;
	meshData.Indices32[10]	= 7;
	meshData.Indices32[11]	= 6;

	// Left face
	meshData.Indices32[12]	= 4;
	meshData.Indices32[13]	= 5;
	meshData.Indices32[14]	= 1;
	meshData.Indices32[15]	= 4;
	meshData.Indices32[16]	= 1;
	meshData.Indices32[17]	= 0;

	// Right face
	meshData.Indices32[18]	= 3;
	meshData.Indices32[19]	= 2;
	meshData.Indices32[20]	= 6;
	meshData.Indices32[21]	= 3;
	meshData.Indices32[22]	= 6;
	meshData.Indices32[23]	= 7;

	// Top face
	meshData.Indices32[24]	= 1;
	meshData.Indices32[25]	= 5;
	meshData.Indices32[26]	= 6;
	meshData.Indices32[27]	= 1;
	meshData.Indices32[28]	= 6;
	meshData.Indices32[29]	= 2;

	// Bottom face
	meshData.Indices32[30]	= 4;
	meshData.Indices32[31]	= 0;
	meshData.Indices32[32]	= 3;
	meshData.Indices32[33]	= 4;
	meshData.Indices32[34]	= 3;
	meshData.Indices32[35]	= 7;

	return meshData;
}

MeshData
GeometryGenerator::CreateGrid(float width, float depth, UINT32 m, UINT32 n, std::string name)
{
	MeshData meshData(name);

	UINT32 vertexCount = m * n;

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
			meshData.Indices32[k]				= (i * n) + j;
			meshData.Indices32[(size_t)k + 1]	= (i * n) + j + 1;
			
			k += 2;
		}
	}

	for (UINT32 i = 0; i < m - 1; ++i)
	{
		for (UINT32 j = 0; j < n; ++j)
		{
			meshData.Indices32[k]				= (i * n) + j;
			meshData.Indices32[(size_t)k + 1]	= ((i + 1) * n) + j;

			k += 2;
		}
	}

	return meshData;
}
