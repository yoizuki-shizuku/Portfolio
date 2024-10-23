#include "BulletEffect.hlsli"

float4 main(PS_INPUT input) : SV_TARGET
{
	//	�J�����֌������x�N�g�����v�Z
    float3 viewDirection = EyePosition - Position;
    viewDirection = normalize(viewDirection);
	
	//	�t���l������
    float3 fresnel = 1 - dot(viewDirection, input.normal);
	
	//	�R���g���X�g�̒���
    fresnel = pow(fresnel, 1.f);
	
	//	�F��t����
    float4 color1 = float4(0.4f, 0.3f, 1.f, 1.f);
    float4 color2 = float4(0.6f, 0.6f, 0.9f, 0.4f);
	
    float4 output = lerp(color2, color1, fresnel.x);
	
    return output;
}
