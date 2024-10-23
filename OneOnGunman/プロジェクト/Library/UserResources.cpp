/*/
 *	ファイル名		：	UserResources.cpp
 *	概要			：	各シーンで共通のものにアクセスできるようにしたシングルトン
 *
 *	作成者			：	山本玄臣
 *	作成日			：	2023/06/05
/*/
#include "pch.h"
#include "UserResources.h"

std::unique_ptr<UserResources> UserResources::s_instance = nullptr;

//	インスタンス生成
void UserResources::CreateInstance()
{
	//	生成されていなければインスタンスを生成
	if (s_instance == nullptr)
	{
		s_instance.reset(new UserResources());
	}
}

//	インスタンス取得
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

