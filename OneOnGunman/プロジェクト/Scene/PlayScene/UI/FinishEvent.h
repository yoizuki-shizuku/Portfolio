/*/
 *  ファイル名		：	FinishEvent.h
 *  概要			：	ゲーム終了時の演出
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/02
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include "Scene/Observer/Observer.h"
#include <array>

class FinishEvent
{
public:
	enum class Process
	{
		MAINTAIN = -1,
		RETRY    = 0,
		SELECT   = 1,
		CLEAR    = 2
	};

public:
	FinishEvent();
	~FinishEvent() = default;

	void Initialize();
	Process Update();
	void Render();

	//	リトライ時のフラグ初期化
	void RetryGame();

public:
	Observer* GetClearObserver() { return m_clearObs.get(); }
	Observer* GetFailureObserver() { return m_failureObs.get(); }

private:
	//	クリア
	std::unique_ptr<UserInterface> m_clear;
	//	失敗
	std::unique_ptr<UserInterface> m_failure;

	//	リトライボタン
	std::unique_ptr<UserInterface> m_retry;
	//	セレクトへ戻るボタン
	std::unique_ptr<UserInterface> m_select;

	//	レターボックス
	std::unique_ptr<UserInterface> m_letterBox_1;
	std::unique_ptr<UserInterface> m_letterBox_2;

	//	クリアアニメーション
	std::unique_ptr<Animator> m_clearAnim;
	//	失敗アニメーション
	std::unique_ptr<Animator> m_failureAnim;

	//	フェードインアニメーション
	std::array<std::unique_ptr<Animator>, 2> m_FadeIn;
	//	選択時アニメーション
	std::array<std::unique_ptr<Animator>, 2> m_choise;
	//	ボタン
	std::array<std::unique_ptr<Button>, 2> m_button;

	//	クリア時のイベントを監視
	std::unique_ptr<Observer> m_clearObs;
	//	失敗時のイベントを監視
	std::unique_ptr<Observer> m_failureObs;

	//	レターボックスのアニメーションタイマー
	float m_letterBoxTimer;
	//	ボタンを押したかどうか
	bool m_pushButton;

};
