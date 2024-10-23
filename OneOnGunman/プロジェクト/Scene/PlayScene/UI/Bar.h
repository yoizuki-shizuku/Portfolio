/*/
 *  ファイル名		：	Bar.h
 *  概要			：	プレイヤーの各クールタイムを表示するバーUI
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/05
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/Observer/Observer.h"

class Bar
{
public:
	Bar();
	~Bar() = default;

	void Initialize();
	void Update(float shotRatio, float dodgeRatio);
	void Render();

	//	リトライ時のフラグ初期化
	void RetryGame();

	Observer* GetFinishObserver() const { return m_finishObs.get(); }

private:
	//	フレーム枠
	std::unique_ptr<UserInterface> m_barFream;
	//	ショットのバー
	std::unique_ptr<UserInterface> m_shotBar;
	//	回避のバー
	std::unique_ptr<UserInterface> m_dodgeBar;
	
	//	ゲーム終了のイベントを検知
	std::unique_ptr<Observer> m_finishObs;


};