// Unlit Shader

cbuffer WorldPos : register(b0)
{
	float4x4 gWorld;
}

cbuffer Color : register(b1)
{
	float4 color;
}

cbuffer CameraView : register(b2)
{
	float4x4 gViewProj;
}



struct VertexIn
{
	float3 Pos		: POSITION;
};

struct VertexOut
{
	float4 Pos					: SV_POSITION;
};

VertexOut VS_Main(VertexIn vin)
{
	wvp = mul(gWorld, gViewProj);

	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), wvp);
	
	return vout;
}

float4 PS_Main(VertexOut pin) : SV_Target
{
	return color;
}
