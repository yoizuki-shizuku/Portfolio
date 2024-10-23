/*/
 *  ファイル名		：	ClearTime.h
 *  概要			：	クリアタイムを表示
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/19
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Numeral.h"

class ClearTime
{
public:
	ClearTime();
	~ClearTime() = default;

	void Initialize(float clearTime);
	void Update();
	void Render();
	void Finalize();

private:
	//	画像
	std::unique_ptr<UserInterface> m_texture;
	//	数字
	std::unique_ptr<Numeral> m_hundred;
	std::unique_ptr<Numeral> m_ten;
	std::unique_ptr<Numeral> m_one;

	//	クリアタイム
	float m_clearTime;
	//	演出用に表示するタイム
	int m_displayTime;
	//	演出用のタイマー
	float m_directionTimer;

};