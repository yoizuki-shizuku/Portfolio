/*/
 *  ファイル名		：	Input.cpp
 *  概要			：	ステートトラッカーをシングルトン化
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/06/05
/*/
#include "pch.h"
#include "Input.h"

using namespace DirectX;

std::unique_ptr<Input> Input::s_instance = nullptr;

void Input::Initialize()
{
	//	インスタンス生成
	m_kbTracker = std::make_unique<Keyboard::KeyboardStateTracker>();
	m_msTracker = std::make_unique<Mouse::ButtonStateTracker>();
}

void Input::Update()
{
	//	ステートトラッカーを更新
	auto keyboard = Keyboard::Get().GetState();
	m_kbTracker->Update(keyboard);
	auto mouse = Mouse::Get().GetState();
	m_msTracker->Update(mouse);
}

void Input::CreateInstance()
{
	//	生成されていなければインスタンスを生成
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
