#include "BulletEffect.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.position = mul(input.position, WorldViewProj);
    output.normal = mul(float4(input.normal, 1.f), Rotate).xyz;
    output.tex = input.tex;
	
    return output;
}