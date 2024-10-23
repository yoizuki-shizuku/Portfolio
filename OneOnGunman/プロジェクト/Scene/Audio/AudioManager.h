/*/
 *  �t�@�C����		�F	AudioManager.h
 *  �T�v			�F	���ʐݒ�ɑ����āABGM��SE���Đ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2024/01/28
/*/
#pragma once

#include <cri_le_xpt.h>
#include <cri_le_atom_ex.h>
#include <cri_le_atom_wasapi.h>

#include "OneOnGunman_acf.h"
#include "BGM.h"
#include "SE.h"

class AudioManager
{
public:
	~AudioManager() = default;

	void Initialize();
	void Update();
	void Finalize();

	void StartBGM(CriAtomExCueId id);
	void StopBGM();
	void PlaySE(CriAtomExCueId id);

	void SetVolumeBGM(const float& volume);
	void SetVolumeSE(const float& volume);

	float GetVolumeBGM() { return m_volumeBGM; }
	float GetVolumeSE() { return m_volumeSE; }

public:
	//	�C���X�^���X����
	static void CreateInstance();
	//	�C���X�^���X�擾
	static AudioManager* const GetInstance();

	//	����A�R�s�[�̃R���X�g���N�^���폜
	void operator=(const AudioManager& object) = delete;
	AudioManager(const AudioManager& object) = delete;

private:
	AudioManager();

private:
	//	�C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<AudioManager> s_instance;

	//	�v���[���[�X�e�[�^�X�@�F�@�v���[���[�̍Đ���Ԃ������l
	CriAtomExPlayerHn m_playerBGM, m_playerSE;
	//	�{�C�X�v�[���n���h���@�F�@�{�C�X�v�[���𐧌䂷�邽�߂̃n���h��
	CriAtomExVoicePoolHn m_pool;
	//	�L���[ID�@�F�@acb�t�@�C������ǂݍ���Audio�f�[�^
	CriAtomExAcbHn m_acb_hn_BGM, m_acb_hn_SE;
	//	D-BAS	:	�X�g���[�~���O�Đ��ɕK�v�ȃo�b�t�@
	CriAtomDbasId m_dbas;

	//	����
	float m_volumeBGM, m_volumeSE;

};