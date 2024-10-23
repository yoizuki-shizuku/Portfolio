#include "Numeral.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //  �摜�̓ǂݍ��݂Ɠ����x�̐ݒ�
    float4 output = tex.Sample(samLinear, input.tex);
    output.a *= alpha;
    
    return output;
}