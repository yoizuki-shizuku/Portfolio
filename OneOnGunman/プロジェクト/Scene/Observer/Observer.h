/*/
 *  ファイル名		：	Observer.h
 *  概要			：	Subjectから通知を受け取る
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/05
/*/
#pragma once

class Subject;

__interface IObserver
{
	void Notify();
};

class Observer : public IObserver
{
public:
	Observer();
	virtual ~Observer() = default;

	//	Subjectからの通知を受け取る
	void Notify() override;

	//	通知を受け取ったのかどうかを確認する
	bool GetNotification() const { return m_notification; }

	//	通知をリセット
	void Reset() { m_notification = false; }

private:
	//	通知を受け取ったかどうか
	bool m_notification;

};