/*/
 *  ファイル名		：	Button.cpp
 *  概要			：	マウスで操作できるUIのボタン
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/09/11
/*/
#include "pch.h"
#include "Button.h"

using namespace DirectX;

#include "Library/Input.h"

Button::Button(UserInterface* ui) :
	m_texture(ui),
	m_onTexture(false),
	m_beforeState(false),
	m_clickButton(false),
	m_beforeButton(false)
{
}

void Button::Update()
{
	//	1F前の状態を保持
	m_beforeState = m_onTexture;
	m_beforeButton = m_clickButton;

	//	マウスクラスを取得
	auto mouse = Mouse::Get().GetState();

	//	マウスの位置を取得
	SimpleMath::Vector2 mousePos;
	mousePos.x = static_cast<float>(mouse.x);
	mousePos.y = static_cast<float>(mouse.y);

	//	UserInterfaceから必要な情報を取得
	SimpleMath::Vector2 position = m_texture->GetPosition();
	SimpleMath::Vector2 size = m_texture->GetSize();
	SimpleMath::Vector2 scale = m_texture->GetScale();
	Anchor anchor = m_texture->GetAnchor();

	//	画像の左上と右下の頂点の位置を割り出す
	SimpleMath::Vector4 vertex;
	vertex.x = position.x;						//	左上x
	vertex.y = position.y;						//	左上y
	vertex.z = position.x + (size.x * scale.x);	//	右下x
	vertex.w = position.y + (size.y * scale.y);	//	右下y

	//	アンカーポイント分位置をずらす
	SimpleMath::Vector2 offset;
	offset.x = (size.x * scale.x / 2.f) * (static_cast<int>(anchor) % 3);
	offset.y = (size.y * scale.y / 2.f) * (static_cast<int>(anchor) / 3);
	vertex -= SimpleMath::Vector4(offset.x, offset.y, offset.x, offset.y);

	//	判定を行う
	m_onTexture = (mousePos.x >= vertex.x) && (mousePos.y >= vertex.y) && (mousePos.x <= vertex.z) && (mousePos.y <= vertex.w);

	//	ボタンをクリックしているかどうかを判定する
	if (m_onTexture && mouse.leftButton) m_clickButton = true;
	else m_clickButton = false;
}

