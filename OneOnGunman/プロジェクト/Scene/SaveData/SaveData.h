/*/
 *  �t�@�C����		�F	SaveData.h
 *  �T�v			�F	�Z�[�u�f�[�^�̓ǂݏ������s��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/01/09
/*/
#pragma once

#include "picojson.h"
#include <array>

struct StageData
{
	int time;
	bool oneShot;
	int tryCount;
};

class SaveData
{
public:
	~SaveData() = default;

	//	�Z�[�u�f�[�^��ǂݍ���
	void LoadSaveData();

	//	�Z�[�u�f�[�^����������
	void WriteSaveData();

	//	�X�e�[�W�̃f�[�^���擾(�ꊇ)
	std::array<StageData, 3> GetStageData() { return m_stageDatas; }
	//	�X�e�[�W�̃f�[�^���擾(�w��)
	StageData GetStageData(int index) { return m_stageDatas[index]; }

	//	�X�e�[�W�̃f�[�^���X�V
	void SuccsesOneShot(int index) { m_stageDatas[index].oneShot = true; }
	void SetTime(int index, int time);
	void AddTryCount(int index);

	//	�e�{�����[���̎擾�ƍX�V
	float GetVolumeBGM() { return m_volumeBGM; }
	float GetVolumeSE() { return m_volumeSE; }
	void SetVolumeBGM(const float& volume) { m_volumeBGM = volume; }
	void SetVolumeSE(const float& volume) { m_volumeSE = volume; }

	//	�f�[�^���Z�b�g
	void ResetData();

public:
	//	�C���X�^���X����
	static void CreateInstance();

	//	�C���X�^���X�擾
	static SaveData* const GetInstance();

private:
	SaveData();

	void operator=(const SaveData& object) = delete;
	SaveData(const SaveData& object) = delete;

private:
	//	�X�e�[�W�̐�
	const static int STAGE_NUM = 3;

	//	�C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<SaveData> s_instance;

	//	�X�e�[�W�̃Z�[�u�f�[�^
	std::array<StageData, STAGE_NUM> m_stageDatas;

	//	�e����
	float m_volumeBGM, m_volumeSE;

};