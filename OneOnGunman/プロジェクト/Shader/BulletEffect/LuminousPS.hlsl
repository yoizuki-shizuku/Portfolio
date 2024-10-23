#include "BulletEffect.hlsli"
#include "Function.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
	//	�J�����֌������x�N�g�����v�Z
    float3 viewDirection = EyePosition - Position;
    viewDirection = normalize(viewDirection);
	
	//	�t���l������
    float3 fresnel = dot(viewDirection, input.normal);
    
    float3 blendColor = float3(0.f, 1.f, 1.f); //   cyan
    float a = smoothstep(0.5f, 1.f, fresnel.x);
    
    float3 color = blendOverlay(fresnel, blendColor, 1);
    
    return float4(color, a);
}