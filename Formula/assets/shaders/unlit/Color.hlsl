// Unlit Shader

cbuffer GlobalProperties : register(b0)
{
	float4x4 gWorld;
}

cbuffer ObjectProperties: register(b1)
{
	float4x4 gViewProj;
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

VertexOut VS_Main(VertexIn vin)
{
	float4x4 wvp = mul(gWorld, gViewProj);

	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), wvp);
	
	return vout;
}

float4 PS_Main(VertexOut pin) : SV_Target
{
	return color;
}
