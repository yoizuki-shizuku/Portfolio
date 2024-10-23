#include "EnemyBullet.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.position = mul(input.position, WorldViewProj);
    output.normal = input.normal;
    output.tex = input.tex;
	
    return output;
}