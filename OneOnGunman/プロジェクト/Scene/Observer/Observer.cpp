/*/
 *  �t�@�C����		�F	Observer.cpp
 *  �T�v			�F	Subject����ʒm���󂯎��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/05
/*/
#include "pch.h"
#include "Observer.h"

Observer::Observer() :
	m_notification(false)
{
}

void Observer::Notify()
{
	m_notification = true;
}
