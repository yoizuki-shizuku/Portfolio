/*/
 *  ファイル名		：	MouseUI.h
 *  概要			：	マウスボタンの操作ヘルプUI
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/12/10
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/Observer/Observer.h"

class MouseUI
{
public:
	MouseUI();
	~MouseUI() = default;

	void Initialize();
	void Update();
	void Render(float shotCoolRatio, float dodgeCoolRatio);

	void RetryGame();

	Observer* GetObserver() { return m_observer.get(); }

private:
	//	マウスボタンの背景
	std::unique_ptr<UserInterface> m_mouseBack;
	//	マウスのボタン
	std::unique_ptr<UserInterface> m_leftButton;
	std::unique_ptr<UserInterface> m_rightButton;
	//	アイコン
	std::unique_ptr<UserInterface> m_shotIcon;
	std::unique_ptr<UserInterface> m_dodgeIcon;
	//	ゲージ
	std::unique_ptr<UserInterface> m_geuge;

	//	Updateが実行されて、表示し始める
	bool m_isDisplay;

	//	クリアまたはミスの通知を受け取る
	std::unique_ptr<Observer> m_observer;

};