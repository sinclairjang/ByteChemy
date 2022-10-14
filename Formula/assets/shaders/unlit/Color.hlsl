// Unlit Shader

cbuffer WorldPos : register(b0)
{
	float4x4 gWorld;
}

cbuffer CameraView : register(b1)
{
	float4x4 gViewProj;
}

cbuffer Color : register(b2)
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
	wvp = mul(gWorld, gViewProj);

	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), wvp);
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return color;
}
