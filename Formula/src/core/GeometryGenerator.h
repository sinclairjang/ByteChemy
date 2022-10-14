#pragma once

class GeometryGenerator
{
public:
	static MeshData& CreateCube();

	static MeshData& CreateGrid(float width, float depth, UINT32 m, UINT32 n);

	static MeshData& CreateCylinder(
		float bottomRadius, float topRadius,
		float height, UINT32 sliceCount, UINT32 stackCount);

	static void BuildCylinderTopCap(
		float bottomRadius, float topRadius, float height,
		UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);

	static void BuildCylinderBottomCap(
		float bottomRadius, float topRadius, float height,
		UINT32 sliceCount, UINT32 stackCount, MeshData& meshData);

	static MeshData& CreateSphere(
		float bottomRadius, float topRadius,
		float height, UINT32 sliceCount, UINT32 stackCount);

	static MeshData& CreateGeoSphere(float radius, UINT32 numSubdivisions);

	static MeshData& CreateFBXGeometryFromFile(const std::wstring& path);
};
