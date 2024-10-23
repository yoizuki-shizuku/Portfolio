/*/
 *  ファイル名		：	State.h
 *  概要			：	ステートパターンで各ステートが継承するクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/09
/*/
#pragma once

#include "Animation.h"

class State
{
public:
	State();
	virtual ~State() = default;

	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void AnimeUpdate(float elapsedTime) final;
	virtual void Render(DirectX::SimpleMath::Matrix world,
						DirectX::SimpleMath::Matrix view,
						DirectX::SimpleMath::Matrix proj);
	virtual void Finalize() = 0;

	//	モデルのアニメーション設定（共通部分を関数化）
	virtual void SetModel(DirectX::Model* model, const wchar_t* fileName) final;

	//	アニメーションの開始時間を設定
	virtual void SetAnimStartTime(const double& startTime) final { m_animSdk.SetStartTime(startTime); }
	//	アニメーションの終了時間を設定
	virtual void SetAnimEndTime(const double& endTime)     final { m_animSdk.SetEndTime(endTime); }
	//	アニメーションがループするかを設定
	virtual void SetAnimLoop(const bool& loop)             final { m_animSdk.SetLoop(loop); }
	//	アニメーションをはじめから再生
	virtual void RestartAnimetion()                        final { m_animSdk.RestartAnim(); }
	//	アニメーションの速度を設定
	virtual void SetAnimSpeed(const float& speed)		   final { m_animSpeed = speed; }

private:
	//	モデル
	DirectX::Model* m_model;
	//	SDKアニメーション
	DX::AnimationSDKMESH m_animSdk;
	//	スケルトン
	DirectX::ModelBone::TransformArray m_bone;
	//	アニメーションスケルトン
	DirectX::ModelBone::TransformArray m_animBone;
	//	ステンシルマスク用
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	//	アニメーションの速度
	float m_animSpeed;

};