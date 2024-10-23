#include "Hologram.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    output.position = input.position;
    output.pos = input.position;
    output.position = mul(output.position, WorldViewProj);
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.color = input.color;
    output.tex = input.tex;
    
    return output;
}