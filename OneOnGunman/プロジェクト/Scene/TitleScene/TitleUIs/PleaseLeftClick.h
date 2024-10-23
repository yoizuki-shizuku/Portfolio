/*/
 *  ファイル名		：	PleasePressAnyKey.h
 *  概要			：	始めにキーが押されるまでUIを表示し、そのキーの押下を検知する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/08
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"

class PleaseLeftClick
{
public:
	PleaseLeftClick();
	~PleaseLeftClick() = default;

	void Initialize();
	bool Update();
	void Render();
	void Finalize();

private:
	//	テクスチャ
	std::unique_ptr<UserInterface> m_texture;
	//	アニメーション用コマンダー
	std::unique_ptr<Animator> m_fadeIn;

	//	左クリックをしたか
	bool m_isLeftClick;

};