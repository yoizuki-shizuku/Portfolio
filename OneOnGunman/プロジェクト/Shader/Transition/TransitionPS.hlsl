#include "Transition.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    //  出力用
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    
    //  書き換え用
    float2 uv = input.tex;
       
    //  上下を二つに分ける
    uv.y = frac(uv.y * 2);
    
    //  上半分の場合は向きを反転させる
    float cut = lerp(1 - uv.y, uv.y, step(input.tex.y, 0.5f));
    
    //  透明度を変更
    output.a = step(cut, ratio.x);
    
    return output;
}