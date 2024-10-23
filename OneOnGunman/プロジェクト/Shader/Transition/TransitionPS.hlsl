#include "Transition.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
    //  �o�͗p
    float4 output = float4(0.f, 0.f, 0.f, 1.f);
    
    //  ���������p
    float2 uv = input.tex;
       
    //  �㉺���ɕ�����
    uv.y = frac(uv.y * 2);
    
    //  �㔼���̏ꍇ�͌����𔽓]������
    float cut = lerp(1 - uv.y, uv.y, step(input.tex.y, 0.5f));
    
    //  �����x��ύX
    output.a = step(cut, ratio.x);
    
    return output;
}