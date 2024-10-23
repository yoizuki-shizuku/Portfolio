#include "InfinitePlane.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = float4(0.f, 1.f, 0.f, 1.f);
        
    float2 uv = float2(input.tex.x, input.tex.y - time.x * 0.03f);
    uv.x = frac(uv.x * 50);
    uv.y = frac(uv.y * 10);
    
    output.a = step(0.9f, uv.y);
    output.a += step(0.95f, uv.x);
    
    return output;
}