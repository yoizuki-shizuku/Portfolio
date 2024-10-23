#include "GlitchEffect.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //  UV���W�����炷�ʒu�����߂�
    float2 uv = input.tex;
    float offsetPos = input.color.r;
    float offsetWidth = input.color.g;
    
    //  �^����ꂽ�����`+0.05�͈̔͂Ȃ�UV�����炷
    uv.x += offsetWidth * step(offsetPos, input.tex.y);
    uv.x -= offsetWidth * step(offsetPos + 0.05f, input.tex.y);
    
    //  �摜��ǂݍ���
    float4 output = tex.Sample(samLinear, uv);
        
    //  �F�����炵
    float offsetColor = windowSize.z;
    
    //  �ԐF
    float2 of = uv;
    of -= 0.005f + offsetColor;
    float4 hue = tex.Sample(samLinear, of);
    
    output.r = hue.a > 0.5f && output.a < 0.5f ? 1.f : output.r;
    output.a = hue.a > 0.5f && output.a < 0.5f ? 0.7f : output.a;
    
    //  �ΐF
    //of = uv;
    //of += offsetColor;
    //hue = tex.Sample(samLinear, of);
    
    //output.g = hue.a > 0.5f && output.a < 0.5f ? 1.f : output.g;
    //output.a = hue.a > 0.5f && output.a < 0.5f ? 0.7f : output.a;
    
    //  �F
    of = uv;
    of += 0.005f - offsetColor;
    hue = tex.Sample(samLinear, of);
    
    output.b = hue.a > 0.5f && output.a < 0.5f ? 1.f : output.b;
    output.a = hue.a > 0.5f && output.a < 0.5f ? 0.7f : output.a;
    
    return output;
}