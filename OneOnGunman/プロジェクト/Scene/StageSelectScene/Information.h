/*/
 *  ファイル名		：	Information.h
 *  概要			：	ステージの情報を表示
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2023/11/22
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
	//	「Stage」の表記
	std::unique_ptr<UserInterface> m_stage;
	//	ステージ番号
	std::unique_ptr<Numeral> m_stageNumber;
	//	ステージ情報の枠
	std::unique_ptr<GlitchEffect> m_fream;
	//	「Time」の表記
	std::unique_ptr<UserInterface> m_time;
	//	クリア時間表記
	std::array<std::unique_ptr<Numeral>, 3> m_timeNumber;
	//	「OneShot」の表記
	std::unique_ptr<UserInterface> m_oneShot;
	//	「〇」の表記
	std::unique_ptr<UserInterface> m_circle;
	//	「×」の表記
	std::unique_ptr<UserInterface> m_cross;
	//	「Play」の表記
	std::unique_ptr<UserInterface> m_play;
	//	プレイ回数表記
	std::array<std::unique_ptr<Numeral>, 3> m_playNumber;

	std::array<StageData, 3> m_stageData;
};