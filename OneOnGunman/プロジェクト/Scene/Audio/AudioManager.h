/*/
 *  ファイル名		：	AudioManager.h
 *  概要			：	音量設定に則して、BGMやSEを再生する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/01/28
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
	//	インスタンス生成
	static void CreateInstance();
	//	インスタンス取得
	static AudioManager* const GetInstance();

	//	代入、コピーのコンストラクタを削除
	void operator=(const AudioManager& object) = delete;
	AudioManager(const AudioManager& object) = delete;

private:
	AudioManager();

private:
	//	インスタンスへのポインタ
	static std::unique_ptr<AudioManager> s_instance;

	//	プレーヤーステータス　：　プレーヤーの再生状態を示す値
	CriAtomExPlayerHn m_playerBGM, m_playerSE;
	//	ボイスプールハンドル　：　ボイスプールを制御するためのハンドル
	CriAtomExVoicePoolHn m_pool;
	//	キューID　：　acbファイルから読み込んだAudioデータ
	CriAtomExAcbHn m_acb_hn_BGM, m_acb_hn_SE;
	//	D-BAS	:	ストリーミング再生に必要なバッファ
	CriAtomDbasId m_dbas;

	//	音量
	float m_volumeBGM, m_volumeSE;

};