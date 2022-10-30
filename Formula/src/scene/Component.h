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

	operator FMMat4() const { return Transform; }

	TransformComponent() = default;
	TransformComponent(const TransformComponent&) = default;
	TransformComponent(const FMMat4& transform) : 
		Transform(transform) {}
};

struct CameraComponent
{
	FMMat4 View{ 1.0f };
	FMMat4 Projection{ 1.0f };

	CameraComponent() = default;
	CameraComponent(const CameraComponent&) = default;
	CameraComponent(const FMMat4& view, const FMMat4& proj)
		: View(view), Projection(proj) {}
};

struct UnlitMaterialComponent
{
	FMVec4 MainColor;

	operator FMVec4() { return MainColor; }

	UnlitMaterialComponent() = default;
	UnlitMaterialComponent(const UnlitMaterialComponent&) = default;
	UnlitMaterialComponent(const FMVec4& color)
		: MainColor(color) {}
};

struct MeshFilterComponent
{
	//TODO: Extract input data from MeshData
};

struct MeshRendererComponent
{
	// Activates mesh render pipeline
	// Shader name = standard (default)
};

struct LineRendererComponent
{
	// Activates line render pipeline
	// Shader name = standard (default)
};

struct ParticleRendererComponent
{
	// Activates particle render pipeline
	// Shader name = standard (default)
};

// Backend render data associated with a game object represented by Entity

// DX12
#ifdef FM_BACKENDS_RENDERER_DIRECTX_12

class MeshGeometry;

struct RenderInfoComponent
{
	// Index to uniform data
	UINT EngineUniformIdx = -1;
	UINT ShadingUniformIdx = -1;

	// Reference to mesh data
	MeshGeometry* meshGeo = nullptr;  //Note: Pointer stability guaranteed

	D3D12_PRIMITIVE_TOPOLOGY primType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	// Parameters for draw call
	UINT ElementCount = 0;
	UINT StartElementLocation = 0;
	int BaseVertexLocation = 0;  //Optional: reserved for DrawIndexedInstanced function
};

// VULKAN
#elif FM_BACKENDS_RENDERER_VULKAN
	
struct RenderInfoComponent
{
	//TODO: ...
};

#endif