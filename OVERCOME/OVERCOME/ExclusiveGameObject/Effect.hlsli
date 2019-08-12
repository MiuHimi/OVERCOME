//////////////////////////////////////////////////////////////
// File.    Effect.hlsli
// Summary. ShaderManagementFile
// Date.    2019/08/12
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

struct VS_INPUT
{
	float3 Pos : POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};
cbuffer ConstBuffer	: register(b0)
{
	matrix matWorld;
	matrix matView;
	matrix matProj;
	float4 diffuse;
	float4 mouse;
	float4 keyboard;
};
