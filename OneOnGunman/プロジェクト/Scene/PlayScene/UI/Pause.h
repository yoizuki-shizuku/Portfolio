/*/
 *  ファイル名		：	Pause.h
 *  概要			：	ポーズ画面
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/12/15
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include <array>

class Pause
{
public:
	enum class Process
	{
		PAUSE = -2,
		MAINTAIN = -1,
		CONTINUE = 0,
		RESTART = 1,
		SELECT = 2
	};


public:
	Pause();
	~Pause() = default;

	void Initialize();
	Process Update();
	void Render();
	void Finalize();

	void RetryGame();

private:
	//	「Pause」の表示
	std::unique_ptr<UserInterface> m_pause;

	//	レターボックス
	std::unique_ptr<UserInterface> m_letterBox_1;
	std::unique_ptr<UserInterface> m_letterBox_2;

	//	配列のサイズ
	static const int SIZE = 3;

	//	ボタンの画像
	std::array<std::unique_ptr<UserInterface>,SIZE> m_texture;
	//	アニメーション
	std::array<std::unique_ptr<Animator>, SIZE> m_animation;
	//	ボタン
	std::array<std::unique_ptr<Button>, SIZE> m_button;

	//	現在行っている処理
	Process m_process;
	//	レターボックスのアニメーションタイマー
	float m_letterBoxTimer;

};