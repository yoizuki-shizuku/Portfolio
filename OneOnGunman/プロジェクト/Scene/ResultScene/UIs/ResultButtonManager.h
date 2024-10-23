/*/
 *  ファイル名		：	ResultButtonManager.h
 *  概要			：	リザルト画面でのボタンを管理する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/18
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include <array>

class ResultButtonManager
{
public:
	enum Process : int
	{
		MAINTAIN = -1,
		SELECT = 0,
		RETRY = 1,
		NEXT = 2,
	};

public:
	ResultButtonManager();
	~ResultButtonManager() = default;

	void Initialize(int stageIndex);
	Process Update();
	void Render();
	void Finalize();

private:
	//	配列のサイズ
	static const int SIZE = 3;

	//	ボタンの画像
	std::array < std::unique_ptr<UserInterface>, SIZE> m_texture;
	//	インフォメーションの画像
	std::array<std::unique_ptr<UserInterface>, SIZE> m_info;

	//	フェードインアニメーション
	std::array <std::unique_ptr<Animator>, SIZE> m_fadeinAnimator;
	//	選択時のアニメーション
	std::array<std::unique_ptr<Animator>, SIZE> m_choiceAnimator;

	//	ボタンの当たり判定
	std::array <std::unique_ptr<Button>, SIZE> m_buttons;

	//	各ボタンのタイミングをずらすためのタイマー
	float m_timer;
	//	最終ステージだったかどうか
	bool m_isLastStage;

};