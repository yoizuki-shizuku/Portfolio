cbuffer ConstBuffer : register(b0)
{
    float windowWidth;
    float windowHeight;
    float alpha;
    uint numeral;
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