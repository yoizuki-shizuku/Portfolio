/*/
 *  �t�@�C����		�F	Observer.h
 *  �T�v			�F	Subject����ʒm���󂯎��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/05
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

	//	Subject����̒ʒm���󂯎��
	void Notify() override;

	//	�ʒm���󂯎�����̂��ǂ������m�F����
	bool GetNotification() const { return m_notification; }

	//	�ʒm�����Z�b�g
	void Reset() { m_notification = false; }

private:
	//	�ʒm���󂯎�������ǂ���
	bool m_notification;

};