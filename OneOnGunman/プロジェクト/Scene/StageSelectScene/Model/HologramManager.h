/*/
 *  ファイル名		：	HologramManager.h
 *  概要			：	選択されているステージと表示するモデルを管理
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/20
/*/
#pragma once

#include "Hologram.h"
#include <array>
#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"

class HologramManager
{
public:
	HologramManager();
	~HologramManager() = default;

	void Initialize(int stageIndex);
	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	int GetNowChoice() const { return m_nowChoice; }

private:
	//	ステージ数
	static const int STAGE_NUM = 3;

	//	敵を表示するホログラム
	std::array < std::unique_ptr<Hologram>, STAGE_NUM> m_holograms;
	//	選択しているステージ
	int m_nowChoice;
	//	ひとつ前に選択していたステージ
	int m_beforeChoice;

	//	選択するための矢印
	std::unique_ptr<UserInterface> m_rightArrow;
	std::unique_ptr<UserInterface> m_leftArrow;
	std::unique_ptr<Button> m_rightArrowButton;
	std::unique_ptr<Button> m_leftArrowButton;
	std::unique_ptr<Animator> m_rightArrowAnimation;
	std::unique_ptr<Animator> m_leftArrowAnimation;

	//	アニメーション用タイマー
	float m_animTimer;
	//	アニメーション中
	bool m_isAnimation;

};