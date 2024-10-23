/*/
 *  ファイル名		：	SaveData.cpp
 *  概要			：	セーブデータの読み書きを行う
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/01/09
/*/
#include "pch.h"
#include "SaveData.h"

#include <fstream>
#include "Library/Useful.h"

std::unique_ptr<SaveData> SaveData::s_instance = nullptr;

void SaveData::CreateInstance()
{
	//	生成されていなければインスタンスを生成
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
	//	ファイルを開く
	std::ifstream ifs;
	ifs.open("Resources/SaveData.json", std::ios::in | std::ios::binary);

	//	開けなければ中断
	assert(ifs);

	//	読み込む
	picojson::value val;
	ifs >> val;

	//	ifs変数はもう使わないので閉じる
	ifs.close();

	//	読み込んだデータをオブジェクトへ変換
	picojson::object obj = val.get<picojson::object>();

	//	ボリュームを確保
	m_volumeBGM = static_cast<float>(obj["VolumeBGM"].get<double>());
	m_volumeSE = static_cast<float>(obj["VolumeSE"].get<double>());

	//	配列に変換
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
	//	先にファイルへ書き込める形式へ変換する
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

	//	ファイルを開く
	std::ofstream ofs;
	ofs.open("Resources/SaveData.json", std::ios::trunc | std::ios::binary);

	//	開けなければ中断
	assert(ofs);

	//	書き込み
	ofs << picojson::value(license).serialize(true);

	//	ファイルを閉じる
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
