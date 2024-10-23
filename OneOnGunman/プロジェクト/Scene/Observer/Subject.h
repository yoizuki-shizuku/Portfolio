/*/
 *  ファイル名		：	Subject.h
 *  概要			：	Observerに通知を送る
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/08/05
/*/
#pragma once
#include <vector>

class Observer;

//	Subjectクラスのインターフェース
__interface ISubject
{
	void Attach(Observer* obs);
	void Detach(Observer* obs);
	void Notify();
};

class Subject : public ISubject
{
public:
	Subject();
	virtual ~Subject() = default;

	//	通知を送るObserverを登録する
	void Attach(Observer* obs)  override final;
	//	Observerへの通知を解除する
	void Detach(Observer* obs) override final;
	//	登録されているObserverに通知を送る
	virtual void Notify() override;
	//	登録されているObserverのフラグをリセット
	void Reset();

private:
	//	通知を送るObserverのリスト
	std::vector<Observer*> m_obsList;

};