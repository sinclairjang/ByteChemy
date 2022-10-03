#pragma once

class GeometryGenerator
{
public:
	struct Vertex; 
	struct MeshData;

	MeshData CreateCube();

	MeshData CreateGrid(float width, float depth, UINT32 m, UINT32 n);

	MeshData CreateCylinder(
		float bottomRadius, float topRadius,
		float height, UINT32 sliceCount, UINT32 stackCount);

		void BuildCylinderTopCap(
			float bottomRadius, float topRadius, float height,
			UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
		
		void BuildCylinderBottomCap(
			float bottomRadius, float topRadius, float height,
			UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);
	
	MeshData CreateSphere(
		float bottomRadius, float topRadius,
		float height, UINT32 sliceCount, UINT32 stackCount);

	MeshData CreateGeoSphere(float radius, UINT32 numSubdivisions);

	MeshData CreateFBXGeometryFromFile(const std::wstring& path);
};

