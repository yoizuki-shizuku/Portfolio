#include "EnemyBullet.hlsli"
#include "Function.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
	//	�J�����֌������x�N�g�����v�Z
    float3 viewDirection = EyePosition - Position.xyz;
    viewDirection = normalize(viewDirection);
	
	//	�t���l������
    float3 fresnel = dot(viewDirection, input.normal);
    
    float3 blendColor = Color.rgb;
    float a = smoothstep(0.1f, 0.7f, fresnel.x);
    
    float3 color = blendOverlay(fresnel, blendColor, 1);
    
    return float4(color, a);
}