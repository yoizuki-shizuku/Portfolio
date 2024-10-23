/*/
 *  �t�@�C����		�F	TreeManager.cpp
 *  �T�v			�F	�r�w�C�r�A�c���[���Ǘ�����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/10
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
