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
	using VertexCount = UINT;
	using IndexCount = UINT;
	using MeshInfo = std::pair<VertexCount, IndexCount>;

	std::string ModelName; // TODO: to be replaced and handled by GUID system
	
	MeshInfo meshInfo;
	
	MeshFilterComponent() = default;
	
	// Opt out costly copy operations
	MeshFilterComponent(const MeshFilterComponent& other) = delete;
	MeshFilterComponent&
		operator=(const MeshFilterComponent&) = default;

	MeshFilterComponent(const std::string name = "Unnamed") 
		: ModelName(name)
	{
	}
};

struct MeshRendererComponent
{
	// activates mesh render pipeline
	// shader name = standard (default)
};

struct LineRendererComponent
{
	// activates line render pipeline
	// shader name = standard (default)
};

struct ParticleRendererComponent
{
	// activates particle render pipeline
	// shader name = standard (default)
};

struct UnlitMatericalComponent
{
	FMVec4 MainColor;

	UnlitMatericalComponent() = default;
	UnlitMatericalComponent(const UnlitMatericalComponent&) = default;
	UnlitMatericalComponent(const FMVec4& color)
		: MainColor(color) {}
};