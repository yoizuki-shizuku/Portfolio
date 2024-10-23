/*/
 *  �t�@�C����		�F	Scene.cpp
 *  �T�v			�F	�V�[���N���X�ŋ��L�̊֐�������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/09
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
	//	�t�F�[�h�C�����I��������A�V�[���J�ڂ��ł���悤�ɂ���
	if (m_isFadeIn && !Transition::GetInstance()->ProcessCheckFadeIn()) m_isFadeIn = false;

	//	�t�F�[�h�A�E�g���I��������A�V�[����J�ڂ���
	if (m_isFadeOut && !Transition::GetInstance()->ProcessCheckFadeOut()) m_isChange = true;
}
