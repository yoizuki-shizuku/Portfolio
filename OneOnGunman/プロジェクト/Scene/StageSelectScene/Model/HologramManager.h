/*/
 *  �t�@�C����		�F	HologramManager.h
 *  �T�v			�F	�I������Ă���X�e�[�W�ƕ\�����郂�f�����Ǘ�
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/20
/*/
#pragma once

#include "Hologram.h"
#include <array>
#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"

class HologramManager
{
public:
	HologramManager();
	~HologramManager() = default;

	void Initialize(int stageIndex);
	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	int GetNowChoice() const { return m_nowChoice; }

private:
	//	�X�e�[�W��
	static const int STAGE_NUM = 3;

	//	�G��\������z���O����
	std::array < std::unique_ptr<Hologram>, STAGE_NUM> m_holograms;
	//	�I�����Ă���X�e�[�W
	int m_nowChoice;
	//	�ЂƂO�ɑI�����Ă����X�e�[�W
	int m_beforeChoice;

	//	�I�����邽�߂̖��
	std::unique_ptr<UserInterface> m_rightArrow;
	std::unique_ptr<UserInterface> m_leftArrow;
	std::unique_ptr<Button> m_rightArrowButton;
	std::unique_ptr<Button> m_leftArrowButton;
	std::unique_ptr<Animator> m_rightArrowAnimation;
	std::unique_ptr<Animator> m_leftArrowAnimation;

	//	�A�j���[�V�����p�^�C�}�[
	float m_animTimer;
	//	�A�j���[�V������
	bool m_isAnimation;

};