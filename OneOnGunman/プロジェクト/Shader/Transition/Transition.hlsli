cbuffer ConstBuffer : register(b0)
{
    float2 windowSize;
    float2 ratio;   //  aにトランジションの進行度
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
