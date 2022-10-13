#pragma once

#include "fm_pch.h"
#include "Scene.h"

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
	TransformComponent(const FMMat4& transform) : 
		Transform(transform) {}
};

struct MeshFilterComponent
{
	MeshData Mesh;
	std::string ModelName; // TODO: to be replaced and handled by GUID system

	MeshFilterComponent() = delete;
	MeshFilterComponent(const MeshFilterComponent& other) = delete;
	MeshFilterComponent(MeshData&& other, std::string name = "Untitled Model") :
		ModelName(name)
	
	{
		Mesh.Vertices = std::move(other.Vertices);
		Mesh.Indices32 = std::move(other.Indices32);
	}
};

struct MeshRendererComponent
{

};

struct ShaderComponent
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