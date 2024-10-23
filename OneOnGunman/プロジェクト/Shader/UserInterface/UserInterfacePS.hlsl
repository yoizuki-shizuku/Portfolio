#include "UserInterface.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //  ‰æ‘œ‚Ì“Ç‚İ‚İ‚Æ“§–¾“x‚Ìİ’è
    float4 output = tex.Sample(samLinear, input.tex);
    output.a *= input.color.a;
    
    //  Š„‡‚Æ•ûŒü‚É‰‚¶‚Ä‰æ‘œ‚Ì•\¦”ÍˆÍ‚ğ§ŒÀ    
    float2 tex = lerp(input.tex, float2(1.f, 1.f) - input.tex, direction % 2);
    
    float2 range = step(tex, float2(ratio, ratio));
    
    output.a *= lerp(range.x, range.y, step(direction, 1.f));
    
    output *= color;
    
    return output;
}