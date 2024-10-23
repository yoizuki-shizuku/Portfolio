#include "BulletEffect.hlsli"
#include "Function.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;
    
    //  ���_���̂̈ʒu
    float4 vertexPosition = input.position;
    //  �m�C�Y�ł��炵����
    float noiseOffset = perlinNoise(input.tex + Time * 2.f, 5.f) * 6.f;
    //  �K�p
    vertexPosition.z = lerp(min(input.position.z, input.position.z * noiseOffset), input.position.z, step(0.f, input.position.z));

    output.position = mul(vertexPosition, WorldViewProj);
    output.normal = mul(float4(input.normal, 1.f), Rotate).xyz;
    output.tex = input.tex;
    
    return output;
}