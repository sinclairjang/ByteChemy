#pragma once

#include "Vu_pch.h"

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
	VuMat4 Transform{ 1.0f };

	operator VuMat4& () { return Transform; }

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const VuMat4& transform)
		: Transform(transform) {}
};

struct MeshComponent
{

};
