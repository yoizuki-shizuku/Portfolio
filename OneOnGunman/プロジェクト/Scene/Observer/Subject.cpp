/*/
 *  ファイル名		：	Subject.cpp
 *  概要			：	Observerに通知を送るオブジェクト
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/05
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
	//	リストに登録されている全てのObserverに通知を送る
	for (Observer* obs : m_obsList)
	{
		obs->Notify();
	}
}

void Subject::Reset()
{
	//	リストに登録されている全てのObserverのフラグをリセット
	for (Observer* obs : m_obsList)
	{
		obs->Reset();
	}
}
