/*/
 *  ファイル名		：	TreeManager.h
 *  概要			：	ビヘイビアツリーを管理するクラス
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/10
/*/
#pragma once

#include "BehaviourTree.h"

//	ビヘイビアツリーを管理するクラス
class TreeManager
{
public:
	TreeManager();
	~TreeManager() = default;

	//	ルートノードの登録
	void Register(Node* node);

	//	ノードの呼び出し
	void Invoke();

	//	Runningで保持しているノードを解除
	void ReleaseRunning();

private:
	//	ルートノード
	std::unique_ptr<Node> m_rootNode;

};
