/*/
 *  �t�@�C����		�F	Information.h
 *  �T�v			�F	�X�e�[�W�̏���\��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/11/22
/*/
#pragma once

#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Numeral.h"
#include "Scene/UserInterface/GlitchEffect.h"
#include <array>
#include "Scene/SaveData/SaveData.h"

class Information
{
public:
	Information();
	~Information() = default;

	void Initialize();
	void Update(int stageIndex);
	void Render();
	void Finalize();

private:
	//	�uStage�v�̕\�L
	std::unique_ptr<UserInterface> m_stage;
	//	�X�e�[�W�ԍ�
	std::unique_ptr<Numeral> m_stageNumber;
	//	�X�e�[�W���̘g
	std::unique_ptr<GlitchEffect> m_fream;
	//	�uTime�v�̕\�L
	std::unique_ptr<UserInterface> m_time;
	//	�N���A���ԕ\�L
	std::array<std::unique_ptr<Numeral>, 3> m_timeNumber;
	//	�uOneShot�v�̕\�L
	std::unique_ptr<UserInterface> m_oneShot;
	//	�u�Z�v�̕\�L
	std::unique_ptr<UserInterface> m_circle;
	//	�u�~�v�̕\�L
	std::unique_ptr<UserInterface> m_cross;
	//	�uPlay�v�̕\�L
	std::unique_ptr<UserInterface> m_play;
	//	�v���C�񐔕\�L
	std::array<std::unique_ptr<Numeral>, 3> m_playNumber;

	std::array<StageData, 3> m_stageData;
};