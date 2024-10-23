/*/
 *  ファイル名		：	MoveUI.h
 *  概要			：	WASDで移動できることとキー入力できているかをを表示するUI
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/12/08
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/Observer/Observer.h"

class MoveUI
{
public:
	MoveUI();
	~MoveUI() = default;

	void Initialize();
	void Update();
	void Render();

	void RetryGame();

	Observer* GetObserver() { return m_observer.get(); }

private:
	//	UIの後ろ側
	std::unique_ptr<UserInterface> m_back;
	//	WASD各種
	std::unique_ptr<UserInterface> m_key_W;
	std::unique_ptr<UserInterface> m_key_A;
	std::unique_ptr<UserInterface> m_key_S;
	std::unique_ptr<UserInterface> m_key_D;

	//	Updateが実行されて、表示し始める
	bool m_isDisplay;

	//	クリアまたはミスの通知を受け取る
	std::unique_ptr<Observer> m_observer;

};