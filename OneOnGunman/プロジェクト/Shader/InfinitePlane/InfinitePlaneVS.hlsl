#include "InfinitePlane.hlsli"

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT) 0;
    output.pos = mul(float4(input.pos, 1.f), world);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    
    output.tex = input.tex;
	
	return output;
}