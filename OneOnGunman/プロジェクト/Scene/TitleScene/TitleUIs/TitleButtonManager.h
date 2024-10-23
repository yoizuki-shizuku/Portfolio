/*/
 *  ファイル名		：	TitleButtonManager.h
 *  概要			：	タイトル画面でのボタンを管理する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/08
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include <array>

class TitleButtonManager
{
public:
	enum Process
	{
		MAINTAIN = -1,
		START	 = 0,
		OPTION	 = 1,
		CREDIT	 = 2,
		EXIT	 = 3
	};

public:
	TitleButtonManager();
	~TitleButtonManager() = default;

	void Initialize();
	int Update();
	void Render();
	void Finalize();

private:
	//	配列のサイズ
	static const int SIZE = 4;

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
};