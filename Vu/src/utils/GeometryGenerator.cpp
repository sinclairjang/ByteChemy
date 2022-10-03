#include "Vu_pch.h"
#include "GeometryGenerator.h"

// Unit testing of implicit casting between VuVecN and DirectXMath math types
/*
void ImplicitCastingTest()
{
	DirectX::XMFLOAT4 xmf4(0.0f, 0.0f, 0.0f, 0.0f);
	VuVec4 vv4(xmf4);
	vv4[0] = 0.f;
	xmf4 = vv4;
}
*/

struct GeometryGenerator::Vertex
{
	Vertex() {}
	Vertex(
		const VuVec3& p,
		const VuVec3& n,
		const VuVec3& t,
		const VuVec2& uv) :
		Position(p),
		Normal(n),
		TangentU(t),
		TexC(uv) {}
	Vertex(
		float px, float py, float pz,
		float nx, float ny, float nz,
		float tx, float ty, float tz,
		float u, float v) :
		Position(px, py, pz),
		Normal(nx, ny, nz),
		TangentU(tx, ty, tz),
		TexC(u, v) {}

	VuVec3 Position;
	VuVec3 Normal;
	VuVec3 TangentU;
	VuVec2 TexC;
};

struct GeometryGenerator::MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<UINT32> Indices32;

	std::vector<UINT16>& GetIndices16()
	{
		if (m_Indices16.empty())
		{
			m_Indices16.resize(Indices32.size());
			for (size_t i = 0; i < m_Indices16.size(); ++i)
			{
				m_Indices16[i] = static_cast<UINT16>(Indices32[i]);
			}
		}
	}
private:
	std::vector<UINT16> m_Indices16;
};

GeometryGenerator::MeshData
GeometryGenerator::CreateGrid(float width, float depth, UINT32 m, UINT32 n)
{
	MeshData meshData;

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

			meshData.Vertices[(i * n) + j].Position = VuVec3(x, 0.0f, z);
			meshData.Vertices[(i * n) + j].Normal	= VuVec3(0.0f, 1.0f, 0.0f);
			meshData.Vertices[(i * n) + j].TangentU = VuVec3(1.0f, 0.0f, 0.0f);
			
			meshData.Vertices[(i * n) + j].TexC.x = j * du;
			meshData.Vertices[(i * n) + j].TexC.y = i * dv;
		}
	}

	UINT32 indexCount = m * (2 * (n - 1)) + n * (2 * (m - 1));
	
	meshData.Indices32.resize(indexCount);

	UINT32 k = 0;
	for (UINT32 i = 0; i < m; ++i)
	{
		for (UINT32 j = 0; j < n - 1; ++j)
		{
			meshData.Indices32[k]		= (i * n) + j;
			meshData.Indices32[k + 1]	= (i * n) + j + 1;
			
			k += 2;
		}
	}

	for (UINT32 i = 0; i < m - 1; ++i)
	{
		for (UINT32 j = 0; j < n; ++j)
		{
			meshData.Indices32[k] = (i * n) + j;
			meshData.Indices32[k + 1] = ((i + 1) * n) + j;

			k += 2;
		}
	}

	return meshData;
}