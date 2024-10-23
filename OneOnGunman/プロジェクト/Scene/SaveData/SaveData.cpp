/*/
 *  �t�@�C����		�F	SaveData.cpp
 *  �T�v			�F	�Z�[�u�f�[�^�̓ǂݏ������s��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/01/09
/*/
#include "pch.h"
#include "SaveData.h"

#include <fstream>
#include "Library/Useful.h"

std::unique_ptr<SaveData> SaveData::s_instance = nullptr;

void SaveData::CreateInstance()
{
	//	��������Ă��Ȃ���΃C���X�^���X�𐶐�
	if (s_instance == nullptr)
	{
		s_instance.reset(new SaveData());
	}
}

SaveData* const SaveData::GetInstance()
{
	return  s_instance.get();
}

SaveData::SaveData()
{
}

void SaveData::LoadSaveData()
{
	//	�t�@�C�����J��
	std::ifstream ifs;
	ifs.open("Resources/SaveData.json", std::ios::in | std::ios::binary);

	//	�J���Ȃ���Β��f
	assert(ifs);

	//	�ǂݍ���
	picojson::value val;
	ifs >> val;

	//	ifs�ϐ��͂����g��Ȃ��̂ŕ���
	ifs.close();

	//	�ǂݍ��񂾃f�[�^���I�u�W�F�N�g�֕ϊ�
	picojson::object obj = val.get<picojson::object>();

	//	�{�����[�����m��
	m_volumeBGM = static_cast<float>(obj["VolumeBGM"].get<double>());
	m_volumeSE = static_cast<float>(obj["VolumeSE"].get<double>());

	//	�z��ɕϊ�
	picojson::array dataList = obj["Stage"].get<picojson::array>();
	for (int i = 0; i < STAGE_NUM; i++)
	{
		picojson::object data = dataList[i].get<picojson::object>();

		m_stageDatas[i].time     = static_cast<int>(data["Time"].get<double>());
		m_stageDatas[i].oneShot  = static_cast<int>(data["OneShot"].get<bool>());
		m_stageDatas[i].tryCount = static_cast<int>(data["TryCount"].get<double>());
	}
}

void SaveData::WriteSaveData()
{
	//	��Ƀt�@�C���֏������߂�`���֕ϊ�����
	picojson::object license;
	picojson::array dataList;

	for (int i = 0; i < STAGE_NUM; i++)
	{
		picojson::object data;
		data.insert(std::make_pair("Time", picojson::value(static_cast<double>(m_stageDatas[i].time))));
		data.insert(std::make_pair("OneShot", picojson::value(m_stageDatas[i].oneShot)));
		data.insert(std::make_pair("TryCount", picojson::value(static_cast<double>(m_stageDatas[i].tryCount))));

		dataList.push_back(picojson::value(data));
	}

	license.insert(std::make_pair("Stage", picojson::value(dataList)));
	license.insert(std::make_pair("VolumeBGM", picojson::value(m_volumeBGM)));
	license.insert(std::make_pair("VolumeSE", picojson::value(m_volumeSE)));

	//	�t�@�C�����J��
	std::ofstream ofs;
	ofs.open("Resources/SaveData.json", std::ios::trunc | std::ios::binary);

	//	�J���Ȃ���Β��f
	assert(ofs);

	//	��������
	ofs << picojson::value(license).serialize(true);

	//	�t�@�C�������
	ofs.close();
}

void SaveData::SetTime(int index, int time)
{
	int saveTime = m_stageDatas[index].time;

	if (saveTime == 0 || saveTime > time)
	{
		m_stageDatas[index].time = Useful::Clamp(time, 0, 999);
	}
}

void SaveData::AddTryCount(int index)
{
	if (m_stageDatas[index].tryCount < 1000)
	{
		m_stageDatas[index].tryCount++;
	}
}

void SaveData::ResetData()
{
	for (int i = 0; i < STAGE_NUM; i++)
	{
		m_stageDatas[i].time = 0;
		m_stageDatas[i].oneShot = false;
		m_stageDatas[i].tryCount = 0;
	}
}
