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
	// Input Filter
	//TODO: Extract input data per shader applied to a mesh object
	// For the time being, we assume a mesh object is given vertex positions, normals, tangents, uvs and indices.
	using VertexCount = UINT32;
	using IndexCount = UINT32;
	using MeshInfo = std::pair<VertexCount, IndexCount>;

	std::wstring ModelName; // TODO: to be replaced and handled by GUID system
	
	MeshData Mesh;
	MeshInfo meshInfo;
	
	MeshFilterComponent() = default;
	
	// Opt out costly copy operations
	MeshFilterComponent(const MeshFilterComponent& other) = delete;
	MeshFilterComponent&
		operator=(const MeshFilterComponent&) = default;

	MeshFilterComponent(MeshData&& other, std::wstring name = s2ws("Untitled Model")) :
		ModelName(name)
	{
		Mesh.Vertices	 = std::move(other.Vertices);
		Mesh.Indices32	 = std::move(other.Indices32);

		meshInfo.first	 = Mesh.Vertices.size();
		meshInfo.second	 = Mesh.Indices32.size();
	}
};

struct MeshRendererComponent
{
	using MeshIndex = std::weak_ptr<void>;
	
	MeshIndex MeshID;

	MeshRendererComponent() = default;
	MeshRendererComponent(const MeshRendererComponent&) = default;
	MeshRendererComponent(const SafelyCopyablePointer<void>& spMesh) :
		MeshID(spMesh) {}
};

struct UnlitMatericalComponent
{
	using MaterialIndex = std::weak_ptr<void>; 

	MaterialIndex MaterialID;
	FMVec4 MainColor;

	UnlitMatericalComponent() = default;
	UnlitMatericalComponent(const UnlitMatericalComponent&) = default;
	UnlitMatericalComponent(const SafelyCopyablePointer<void>& spShader, const FMVec4& color)
		: MaterialID(spShader), MainColor(color) {}
};