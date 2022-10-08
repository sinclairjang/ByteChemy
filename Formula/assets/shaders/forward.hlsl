cbuffer Transform : register(b0)
{
	float4x4 gWorldViewProj;
}

struct VertexIn
{
	float3 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float3 Tangent	: TANGENT;
	float2 Tex0		: TEXCOORD;
};

struct VertexOut
{
	float4 Pos					: SV_POSITION;
	float3 Normal 				: NORMAL;
	float3 Tangent				: TANGENT;
	float2 Tex0					: TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	float4x4 wvp = mul(gWorld, gViewProj);

	VertexOut vout;
	vout.Pos = mul(float4(vin.Pos, 1.0f), gWorldViewProj);
	vout.Normal = mul(float4(vin.Normal, 0.0f), gWorldViewProj);
	vout.Tangent = mul(float4(vin.Tangent, 0.0f), gWorldViewProj);
	vout.Tex0 = vin.Tex0;
	
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	return float4(pin.Tex0.x, pin.Tex0.y, 0.0f, 1.0f);
}
