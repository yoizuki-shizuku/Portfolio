cbuffer ConstBuffer : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 world;
    float4 time;
}

struct VS_INPUT
{
    float3 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};