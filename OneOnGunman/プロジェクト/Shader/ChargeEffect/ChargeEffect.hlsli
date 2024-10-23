cbuffer ConstBuffer : register(b0)
{
    float timer;
    float3 none;
}

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

struct VS_INPUT
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};
