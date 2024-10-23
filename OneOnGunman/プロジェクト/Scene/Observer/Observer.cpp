/*/
 *  ファイル名		：	Observer.cpp
 *  概要			：	Subjectから通知を受け取る
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/05
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
