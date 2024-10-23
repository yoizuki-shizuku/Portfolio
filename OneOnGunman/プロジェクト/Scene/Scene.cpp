/*/
 *  ファイル名		：	Scene.cpp
 *  概要			：	シーンクラスで共有の関数を実装
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/09
/*/
#include "pch.h"
#include "Scene.h"

Scene::Scene() :
	m_exitGame(false),
	m_isChange(false),
	m_isFadeIn(true),
	m_isFadeOut(false),
	m_nextScene(nullptr)
{
}

void Scene::CommonUpdate()
{
	//	フェードインが終了したら、シーン遷移をできるようにする
	if (m_isFadeIn && !Transition::GetInstance()->ProcessCheckFadeIn()) m_isFadeIn = false;

	//	フェードアウトが終了したら、シーンを遷移する
	if (m_isFadeOut && !Transition::GetInstance()->ProcessCheckFadeOut()) m_isChange = true;
}
