#include "ChargeEffect.hlsli"
#include "../BulletEffect/Function.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = float4(0.f, 0.4f, 1.f, 1.f);
    
    float2 uv = float2(input.tex.x, input.tex.y - timer * 0.05f);
    
    color.a = perlinNoise(uv, 30.f) * 0.3f;
    
    return color;
}