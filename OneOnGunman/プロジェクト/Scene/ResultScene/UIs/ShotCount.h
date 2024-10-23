/*/
 *  ファイル名		：	ShotCount.h
 *  概要			：	射撃回数を表示する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/10/19
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Numeral.h"

class ShotCount
{
public:
	ShotCount();
	~ShotCount() = default;

	void Initialize(int shotCount);
	void Update();
	void Render();
	void Finalize();

private:
	//	「OneShot」の画像
	std::unique_ptr<UserInterface> m_texture;

	//	〇と×
	std::unique_ptr<UserInterface> m_circle;
	std::unique_ptr<UserInterface> m_cross;

	//	射撃回数
	int m_shotCount;

};