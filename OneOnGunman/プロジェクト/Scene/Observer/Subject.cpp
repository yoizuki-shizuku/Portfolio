/*/
 *  �t�@�C����		�F	Subject.cpp
 *  �T�v			�F	Observer�ɒʒm�𑗂�I�u�W�F�N�g
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/05
/*/
#include "pch.h"
#include "Subject.h"

#include "Observer.h"

Subject::Subject()
{
	m_obsList.clear();
}

void Subject::Attach(Observer* obs)
{
	m_obsList.emplace_back(obs);
}

void Subject::Detach(Observer* obs)
{
	m_obsList.erase(std::remove(m_obsList.begin(), m_obsList.end(), obs));
}

void Subject::Notify()
{
	//	���X�g�ɓo�^����Ă���S�Ă�Observer�ɒʒm�𑗂�
	for (Observer* obs : m_obsList)
	{
		obs->Notify();
	}
}

void Subject::Reset()
{
	//	���X�g�ɓo�^����Ă���S�Ă�Observer�̃t���O�����Z�b�g
	for (Observer* obs : m_obsList)
	{
		obs->Reset();
	}
}
