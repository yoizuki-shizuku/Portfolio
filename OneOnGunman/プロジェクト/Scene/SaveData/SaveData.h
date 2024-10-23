/*/
 *  ファイル名		：	SaveData.h
 *  概要			：	セーブデータの読み書きを行う
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/01/09
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

	//	セーブデータを読み込む
	void LoadSaveData();

	//	セーブデータを書き込む
	void WriteSaveData();

	//	ステージのデータを取得(一括)
	std::array<StageData, 3> GetStageData() { return m_stageDatas; }
	//	ステージのデータを取得(指定)
	StageData GetStageData(int index) { return m_stageDatas[index]; }

	//	ステージのデータを更新
	void SuccsesOneShot(int index) { m_stageDatas[index].oneShot = true; }
	void SetTime(int index, int time);
	void AddTryCount(int index);

	//	各ボリュームの取得と更新
	float GetVolumeBGM() { return m_volumeBGM; }
	float GetVolumeSE() { return m_volumeSE; }
	void SetVolumeBGM(const float& volume) { m_volumeBGM = volume; }
	void SetVolumeSE(const float& volume) { m_volumeSE = volume; }

	//	データリセット
	void ResetData();

public:
	//	インスタンス生成
	static void CreateInstance();

	//	インスタンス取得
	static SaveData* const GetInstance();

private:
	SaveData();

	void operator=(const SaveData& object) = delete;
	SaveData(const SaveData& object) = delete;

private:
	//	ステージの数
	const static int STAGE_NUM = 3;

	//	インスタンスへのポインタ
	static std::unique_ptr<SaveData> s_instance;

	//	ステージのセーブデータ
	std::array<StageData, STAGE_NUM> m_stageDatas;

	//	各音量
	float m_volumeBGM, m_volumeSE;

};