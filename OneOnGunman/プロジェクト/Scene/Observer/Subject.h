/*/
 *  �t�@�C����		�F	Subject.h
 *  �T�v			�F	Observer�ɒʒm�𑗂�
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/05
/*/
#pragma once
#include <vector>

class Observer;

//	Subject�N���X�̃C���^�[�t�F�[�X
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

	//	�ʒm�𑗂�Observer��o�^����
	void Attach(Observer* obs)  override final;
	//	Observer�ւ̒ʒm����������
	void Detach(Observer* obs) override final;
	//	�o�^����Ă���Observer�ɒʒm�𑗂�
	virtual void Notify() override;
	//	�o�^����Ă���Observer�̃t���O�����Z�b�g
	void Reset();

private:
	//	�ʒm�𑗂�Observer�̃��X�g
	std::vector<Observer*> m_obsList;

};