/*/
 *  ファイル名		：	BeginEvent.h
 *  概要			：	ゲーム開始前の演出
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/01
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"

class BeginEvent
{
public:
	BeginEvent();
	~BeginEvent() = default;

	void Initialize();
	bool Update();
	void Render();
	void Finalize();

	//	リトライ時のフラグ初期化
	void RetryGame();

private:
	//	Readyの文字
	std::unique_ptr<UserInterface> m_ready;
	//	GOの文字
	std::unique_ptr<UserInterface> m_go;
	//	レターボックス
	std::unique_ptr<UserInterface> m_letterBox_1;
	std::unique_ptr<UserInterface> m_letterBox_2;
	//	Readyのアニメーション
	std::unique_ptr<Animator> m_readyAnim;
	//	GOのアニメーション
	std::unique_ptr<Animator> m_goAnim;
	//	レターボックスのアニメーションタイマー
	float m_letterBoxTimer;

};