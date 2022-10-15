#pragma once

#include "fm_pch.h"
#include "Scene.h"
#include "core/GeometryGenerator.h"

struct TagComponent
{
	std::wstring Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::wstring& tag)
		: Tag(tag) {}
};

struct TransformComponent
{
	FMMat4 Transform{ 1.0f };

	operator FMMat4& () { return Transform; }

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const FMMat4& transform) : 
		Transform(transform) {}
};

struct MeshFilterComponent
{
	using VertexCount = UINT32;
	using IndexCount = UINT32;
	using MeshInfo = std::pair<VertexCount, IndexCount>;

	std::wstring ModelName; // TODO: to be replaced and handled by GUID system
	
	MeshData Mesh;
	MeshInfo meshInfo;
	
	MeshFilterComponent() = delete;
	MeshFilterComponent(const MeshFilterComponent& other) = delete;
	MeshFilterComponent(MeshData&& other, std::wstring name = s2ws("Untitled Model")) :
		ModelName(name)
	{
		Mesh.Vertices	 = std::move(other.Vertices);
		Mesh.Indices32	 = std::move(other.Indices32);

		meshInfo.first	 = Mesh.Vertices.size();
		meshInfo.second	 = Mesh.Indices32.size();
	}
};

struct GraphicsComponent
{
	// Driver Informations
	std::wstring GraphicsAPI;

	// Render Informations
	SafelyCopyablePointer<void> MeshHandle;
	SafelyCopyablePointer<void> GraphicsPipelineHandle;
};

struct MeshRendererComponent
{

};

struct MaterialComponent
{

};

struct UnlitMatericalComponent
{
	FMVec4 MainColor;

	UnlitMatericalComponent() = default;
	UnlitMatericalComponent(const UnlitMatericalComponent&) = default;
	UnlitMatericalComponent(const FMVec4 & color)
		: MainColor(color) {}

};