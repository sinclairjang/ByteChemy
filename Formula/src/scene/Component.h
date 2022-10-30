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

struct MainEditorCameraComponent
{

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

struct UnlitMaterialComponent
{
	FMVec4 MainColor;

	UnlitMaterialComponent() = default;
	UnlitMaterialComponent(const UnlitMaterialComponent&) = default;
	UnlitMaterialComponent(const FMVec4& color)
		: MainColor(color) {}
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