#include "BulletEffect.hlsli"
#include "Function.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = float2(input.tex.x, input.tex.y - Time * 1.5f);
    float n = valueNoise(uv * 0.7f, 30.f);
    
    n = Remap(n, float2(0, 1), float2(-3, 3));
    n = abs(n);
    n = 1 - n;
    n = saturate(n);
    
    float a = step(0.8f, n);
        
    float4 output = float4(n, n, n, a);
    output.xyz *= float3(0.f, 1.f, 1.f); //   cyan
    
    return output;
}