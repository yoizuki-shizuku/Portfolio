cbuffer ConstBuffer : register(b0)
{
    float2 windowSize;
    float2 ratio;   //  a�Ƀg�����W�V�����̐i�s�x
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
