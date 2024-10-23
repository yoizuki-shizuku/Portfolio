/*/
 *  �t�@�C����		�F	TreeManager.h
 *  �T�v			�F	�r�w�C�r�A�c���[���Ǘ�����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/10
/*/
#pragma once

#include "BehaviourTree.h"

//	�r�w�C�r�A�c���[���Ǘ�����N���X
class TreeManager
{
public:
	TreeManager();
	~TreeManager() = default;

	//	���[�g�m�[�h�̓o�^
	void Register(Node* node);

	//	�m�[�h�̌Ăяo��
	void Invoke();

	//	Running�ŕێ����Ă���m�[�h������
	void ReleaseRunning();

private:
	//	���[�g�m�[�h
	std::unique_ptr<Node> m_rootNode;

};
