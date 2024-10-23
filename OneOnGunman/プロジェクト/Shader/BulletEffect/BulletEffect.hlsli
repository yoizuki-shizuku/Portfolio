cbuffer Parameters : register(b0)
{
    float4 DiffuseColor : packoffset(c0);
    float3 EmissiveColor : packoffset(c1);
    float3 SpecularColor : packoffset(c2);
    float SpecularPower : packoffset(c2.w);

    float3 LightDirection[3] : packoffset(c3);
    float3 LightDiffuseColor[3] : packoffset(c6);
    float3 LightSpecularColor[3] : packoffset(c9);

    float3 EyePosition : packoffset(c12);

    float3 FogColor : packoffset(c13);
    float4 FogVector : packoffset(c14);

    float4x4 World : packoffset(c15);
    float3x3 WorldInverseTranspose : packoffset(c19);
    float4x4 WorldViewProj : packoffset(c22);
};

cbuffer MyParameters : register(b1)
{
    float3 Position;
    float Time;
    float4x4 Rotate;
}

struct VS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};