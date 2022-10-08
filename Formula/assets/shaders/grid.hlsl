
cbuffer Transform : register(b0)
{
	float4x4 gWorldViewProj;
}

struct VertexIn
{
	float3 Pos		: POSITION;
	float2 uv		: TEXCOORD;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
};

struct VertexOut
{
	float4 Pos					: SV_POSITION;
	float4 Color				: COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	vout.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return pin.Color;
}
