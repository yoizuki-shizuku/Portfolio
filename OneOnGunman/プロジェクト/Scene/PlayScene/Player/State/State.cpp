/*/
 *  ファイル名		：	State.cpp
 *  概要			：	ステートパターンで各ステートが継承するクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/09
/*/
#include "pch.h"
#include "State.h"

using namespace DirectX;

#include "Library/UserResources.h"

State::State() :
	m_model(nullptr),
	m_animSpeed(1.f)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	深度ステンシルの作成 
	D3D11_DEPTH_STENCIL_DESC desc;

	// 描画時０を１にする
	desc.DepthEnable = TRUE;									// 深度テストを行う
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// 深度バッファを更新する
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// 深度値以下なら更新する

	desc.StencilEnable = TRUE;									// ステンシルテストを行う
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// 表面
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// ０なら
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK　１＋
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG　何もしない
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG　何もしない

	desc.BackFace = desc.FrontFace;	// 裏面も同じ

	DX::ThrowIfFailed(
		device->CreateDepthStencilState(&desc, m_depthStencilState.ReleaseAndGetAddressOf())
	);

}

void State::AnimeUpdate(float elapsedTime)
{
	//	アニメーションの更新
	m_animSdk.Update(elapsedTime * m_animSpeed);
	size_t nbones = m_model->bones.size();
	m_animSdk.Apply(*m_model, nbones, m_bone.get());
}

void State::Render(SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	描画に必要な物を取得
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	アニメーションを適用した描画
	size_t nbones = m_model->bones.size();

	m_model->DrawSkinned(
		context,
		*states,
		nbones,
		m_bone.get(),
		world,
		view,
		proj,
		false,
		[&]() {
			//	半透明描画指定
			context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);
			//	カリングは左周り
			context->RSSetState(states->CullClockwise());
			//	深度バッファの設定
			context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
		}
	);
}

void State::SetModel(DirectX::Model* model, const wchar_t* fileName)
{
	//	メンバ変数に確保
	m_model = model;

	//	アニメーションファイルの読み込み
	size_t nbones = m_model->bones.size();
	m_animBone = ModelBone::MakeArray(nbones);
	m_model->CopyBoneTransformsTo(nbones, m_animBone.get());

	m_bone = ModelBone::MakeArray(nbones);
	m_model->CopyBoneTransformsFrom(nbones, m_animBone.get());

	for (size_t i = 0; i < nbones; ++i)
	{
		m_bone[i] = XMMatrixMultiply(m_model->invBindPoseMatrices[i], m_bone[i]);
	}

	//	アニメーションの読み込み
	DX::ThrowIfFailed(
		m_animSdk.Load(fileName)
	);

	m_animSdk.Bind(*m_model);

	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		}
	);
}
