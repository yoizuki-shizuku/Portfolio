/*/
 *  �t�@�C����		�F	SkySphere.cpp
 *  �T�v			�F	�V����\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/26
/*/
#include "pch.h"
#include "SkySphere.h"

using namespace DirectX;

#include "Library/UserResources.h"

SkySphere::SkySphere()
{
}

void SkySphere::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	cmo���f���̐���
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO/");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/SkyShpere.cmo", *fx);

	//	�G�t�F�N�g�̐ݒ�
	m_model->UpdateEffects([&](IEffect* effect)
		{
			IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ���C�g�̉e�����Ȃ���
				lights->SetAmbientLightColor(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}

			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// �G�~�b�V�����F�𔒂ɐݒ肷��
				basicEffect->SetEmissiveColor(SimpleMath::Vector3(1.0f, 1.0f, 1.0f));
			}
		});
}

void SkySphere::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	m_model->Draw(context, *states, world, view, proj);
}

void SkySphere::Finalize()
{
}
