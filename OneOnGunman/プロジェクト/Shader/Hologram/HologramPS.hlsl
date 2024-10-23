#include "Hologram.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 output = tex.Sample(samLinear, input.tex);
    
    output.b += 0.5f;
    output.a = 0.6f;
    float y = frac(input.pos.y * 50.f - Time.x * 10.f);
    y = pow(y, 5.f);
    y = 1.f - y;
    
    output *= float4(y, y, 1.f, 1.f);
            
    return output;
}