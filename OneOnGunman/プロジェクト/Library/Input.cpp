/*/
 *  �t�@�C����		�F	Input.cpp
 *  �T�v			�F	�X�e�[�g�g���b�J�[���V���O���g����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/05
/*/
#include "pch.h"
#include "Input.h"

using namespace DirectX;

std::unique_ptr<Input> Input::s_instance = nullptr;

void Input::Initialize()
{
	//	�C���X�^���X����
	m_kbTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
	m_msTracker = std::make_unique<Mouse::ButtonStateTracker>();
}

void Input::Update()
{
	//	�X�e�[�g�g���b�J�[���X�V
	auto keyboard = Keyboard::Get().GetState();
	m_kbTracker->Update(keyboard);
	auto mouse = Mouse::Get().GetState();
	m_msTracker->Update(mouse);
}

void Input::CreateInstance()
{
	//	��������Ă��Ȃ���΃C���X�^���X�𐶐�
	if (s_instance == nullptr)
	{
		s_instance.reset(new Input());
	}
}

Input* const Input::GetInstace()
{
	return s_instance.get();
}

Input::Input() :
	m_kbTracker(nullptr),
	m_msTracker(nullptr)
{
}
