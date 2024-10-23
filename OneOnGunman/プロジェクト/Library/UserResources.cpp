/*/
 *	�t�@�C����		�F	UserResources.cpp
 *	�T�v			�F	�e�V�[���ŋ��ʂ̂��̂ɃA�N�Z�X�ł���悤�ɂ����V���O���g��
 *
 *	�쐬��			�F	�R�{���b
 *	�쐬��			�F	2023/06/05
/*/
#include "pch.h"
#include "UserResources.h"

std::unique_ptr<UserResources> UserResources::s_instance = nullptr;

//	�C���X�^���X����
void UserResources::CreateInstance()
{
	//	��������Ă��Ȃ���΃C���X�^���X�𐶐�
	if (s_instance == nullptr)
	{
		s_instance.reset(new UserResources());
	}
}

//	�C���X�^���X�擾
UserResources* const UserResources::GetInstance()
{
	return s_instance.get();
}

UserResources::UserResources() :
	m_deviceResources(nullptr),
	m_stepTimer(nullptr),
	m_commonStates(nullptr)
{
}

