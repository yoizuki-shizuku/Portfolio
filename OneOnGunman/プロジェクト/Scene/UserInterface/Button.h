/*/
 *  ファイル名		：	Button.h
 *  概要			：	マウスで操作できるUIのボタン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/11
/*/
#pragma once

#include "UserInterface.h"

class Button
{
public:
	Button(UserInterface* ui);
	~Button() = default;

	void Update();

	//	マウスがテクスチャと重なった瞬間
	bool OnMouse() const { return !m_beforeState && m_onTexture; }
	//	マウスがテクスチャに重なっている
	bool StayMouse() const { return m_onTexture; }
	//	マウスがテクスチャから離れた瞬間
	bool EnterMouse() const { return m_beforeState && !m_onTexture; }
	//	ボタンがクリックされた瞬間
	bool IsPresed() const { return !m_beforeButton && m_clickButton; }
	//	ボタンがクリックされている
	bool IsEnter() const { return m_clickButton; }
	//	ボタンがクリックされた状態から解放された瞬間
	bool IsReleased() const { return m_beforeButton && m_clickButton; }

private:
	//	画像
	UserInterface* m_texture;

	//	マウスがテクスチャと被っているか
	bool m_onTexture;
	//	1F前の状態
	bool m_beforeState;

	//	ボタンが押されているか
	bool m_clickButton;
	//	1f前の状態
	bool m_beforeButton;

};