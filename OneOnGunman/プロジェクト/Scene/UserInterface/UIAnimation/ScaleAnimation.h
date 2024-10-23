/*/
 *  ファイル名		：	ScaleAnimation.h
 *  概要			：	拡縮コマンド
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/07
/*/
#pragma once

#include "../UIAnimation.h"
#include "../UserInterface.h"

class ScaleAnimation : public UIAnimation
{
public:
	ScaleAnimation(UserInterface* ui,
		DirectX::SimpleMath::Vector2 startScale,
		DirectX::SimpleMath::Vector2 endScale,
		float startTime,
		float endTime,
		std::function<float(float)> easing);
	~ScaleAnimation() = default;

	void Execute() override;

private:
	UserInterface* m_userInterface;

	//	開始サイズ
	DirectX::SimpleMath::Vector2 m_startScale;
	//	終了サイズ
	DirectX::SimpleMath::Vector2 m_endScale;

	//	イージング関数
	std::function<float(float)> m_easing;

};