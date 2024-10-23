cbuffer ConstBuffer : register(b0)
{
    float windowWidth;
    float windowHeight;
    float ratio;
    uint direction;
    float4 color;
}

struct VS_INPUT
{
    float3 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float2 tex : TEXCOORD;
};