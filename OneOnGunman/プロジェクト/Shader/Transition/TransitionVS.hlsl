#include "Transition.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    output.pos = input.pos;
    output.tex = input.tex;
    
    return output;
}