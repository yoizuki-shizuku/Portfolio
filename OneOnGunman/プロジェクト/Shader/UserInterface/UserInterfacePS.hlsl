#include "UserInterface.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    //  画像の読み込みと透明度の設定
    float4 output = tex.Sample(samLinear, input.tex);
    output.a *= input.color.a;
    
    //  割合と方向に応じて画像の表示範囲を制限    
    float2 tex = lerp(input.tex, float2(1.f, 1.f) - input.tex, direction % 2);
    
    float2 range = step(tex, float2(ratio, ratio));
    
    output.a *= lerp(range.x, range.y, step(direction, 1.f));
    
    output *= color;
    
    return output;
}