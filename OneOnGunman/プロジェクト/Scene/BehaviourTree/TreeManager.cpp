/*/
 *  ファイル名		：	TreeManager.cpp
 *  概要			：	ビヘイビアツリーを管理するクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/10
/*/
#include "pch.h"
#include "TreeManager.h"

TreeManager::TreeManager()
{
}

void TreeManager::Register(Node* node)
{
	m_rootNode.reset(node);
}

void TreeManager::Invoke()
{
	m_rootNode->Invoke();
}

void TreeManager::ReleaseRunning()
{
	m_rootNode->ReleaseRunning();
}
