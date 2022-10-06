#pragma once

#include "fm_pch.h"

struct TagComponent
{
	std::string Tag;

	TagComponent() = default;
	TagComponent(const TagComponent&) = default;
	TagComponent(const std::string& tag)
		: Tag(tag) {}
};

struct TransformComponent
{
	FMMat4 Transform{ 1.0f };

	operator FMMat4& () { return Transform; }

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const FMMat4& transform)
		: Transform(transform) {}
};

struct MeshComponent
{
	MeshData meshData;

	MeshComponent() = default;
	MeshComponent(const MeshComponent& other) = default;
	MeshComponent(MeshData&& other)
	{
		meshData.Vertices = std::move(other.Vertices);
		meshData.Indices32 = std::move(other.Indices32);
	}
};
