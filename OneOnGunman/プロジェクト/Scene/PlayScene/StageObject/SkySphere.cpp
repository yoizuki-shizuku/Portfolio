/*/
 *  ファイル名		：	SkySphere.cpp
 *  概要			：	天球を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/07/26
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

	//	cmoモデルの生成
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO/");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/SkyShpere.cmo", *fx);

	//	エフェクトの設定
	m_model->UpdateEffects([&](IEffect* effect)
		{
			IEffectLights* lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				// ライトの影響をなくす
				lights->SetAmbientLightColor(SimpleMath::Vector3(0.0f, 0.0f, 0.0f));
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}

			BasicEffect* basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				// エミッション色を白に設定する
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
