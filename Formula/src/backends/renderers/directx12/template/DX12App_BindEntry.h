#pragma once

#include "fm_config.h"
#include <DirectXMath.h>

using namespace DirectX;

// These variants of data will be bound to the common memory scheme (i.e. root signature)
//that defines the set of shader registers to provide consistent interface with the underlying hardware per draw call.

struct EngineObjectProperty		// -> register(b0)
{
	//TEMP: 4x4 identity matrix (row-major)
	XMFLOAT4X4 gWorld = {1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 0, 0, 0, 1};	
};

struct EnginPass					// -> register(b1)
{
	//TEMP
	XMFLOAT4X4 gView;
	XMFLOAT4X4 gInvView;
	XMFLOAT4X4 gProj;
	XMFLOAT4X4 gInvProj;
	XMFLOAT4X4 gViewProj;
	XMFLOAT4X4 gInvViewProj;
	XMFLOAT3 gEyePosW;
	float _Pad1;
};

enum class ShadingType : UINT8
{
	STANDARD,
	UNLIT,
	PARTICLE,
};

//DEBUG MARKER: should I supply placeholder data?
struct UnlitShadingPass			// -> register(b2)
{
	//TEMP
	XMFLOAT4 _Placeholder;
};

// Polymorphic base class
struct ShadingProperty
{
	virtual ~ShadingProperty() = 0;
};

struct UnlitShadingProperty : public ShadingProperty		// -> register(b3)
{
	virtual ~UnlitShadingProperty();

	//TEMP
	XMFLOAT4 gColor;
};

//TODO:
//struct ParticleShadingPass		// -> register(b2)
//{
//
//};
//
//struct ParticleShadingProperties	// -> register(b3)
//{
//
//};
//struct StandardShadingPass		// -> register(b2)
//{
//
//};
//
//struct StandardShadingProperties	// -> register(b3)
//{
//
//};