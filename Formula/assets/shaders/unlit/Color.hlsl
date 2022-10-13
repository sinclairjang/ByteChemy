// Unlit Shader

cbuffer Transform : register(b0)
{
	float4x4 gWorldViewProj;
}

cbuffer Color : register(b1)
{
	float4 color;
}

struct VertexIn
{
	float3 Pos		: POSITION;
};

struct VertexOut
{
	float4 Pos					: SV_POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return color;
}
