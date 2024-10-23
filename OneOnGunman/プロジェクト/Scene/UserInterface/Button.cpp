/*/
 *  �t�@�C����		�F	Button.cpp
 *  �T�v			�F	�}�E�X�ő���ł���UI�̃{�^��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/11
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
	//	1F�O�̏�Ԃ�ێ�
	m_beforeState = m_onTexture;
	m_beforeButton = m_clickButton;

	//	�}�E�X�N���X���擾
	auto mouse = Mouse::Get().GetState();

	//	�}�E�X�̈ʒu���擾
	SimpleMath::Vector2 mousePos;
	mousePos.x = static_cast<float>(mouse.x);
	mousePos.y = static_cast<float>(mouse.y);

	//	UserInterface����K�v�ȏ����擾
	SimpleMath::Vector2 position = m_texture->GetPosition();
	SimpleMath::Vector2 size = m_texture->GetSize();
	SimpleMath::Vector2 scale = m_texture->GetScale();
	Anchor anchor = m_texture->GetAnchor();

	//	�摜�̍���ƉE���̒��_�̈ʒu������o��
	SimpleMath::Vector4 vertex;
	vertex.x = position.x;						//	����x
	vertex.y = position.y;						//	����y
	vertex.z = position.x + (size.x * scale.x);	//	�E��x
	vertex.w = position.y + (size.y * scale.y);	//	�E��y

	//	�A���J�[�|�C���g���ʒu�����炷
	SimpleMath::Vector2 offset;
	offset.x = (size.x * scale.x / 2.f) * (static_cast<int>(anchor) % 3);
	offset.y = (size.y * scale.y / 2.f) * (static_cast<int>(anchor) / 3);
	vertex -= SimpleMath::Vector4(offset.x, offset.y, offset.x, offset.y);

	//	������s��
	m_onTexture = (mousePos.x >= vertex.x) && (mousePos.y >= vertex.y) && (mousePos.x <= vertex.z) && (mousePos.y <= vertex.w);

	//	�{�^�����N���b�N���Ă��邩�ǂ����𔻒肷��
	if (m_onTexture && mouse.leftButton) m_clickButton = true;
	else m_clickButton = false;
}

