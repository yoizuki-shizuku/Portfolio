/*/
 *  ファイル名		：	AudioManager.cpp
 *  概要			：	音量設定に則して、BGMやSEを再生する
 *
 *  作成者			：	山本玄臣
 *  作成日			：	2024/01/28
/*/
#include "pch.h"
#include "AudioManager.h"

using namespace DirectX;

#include "Scene/SaveData/SaveData.h"

#define ACF_FILE			("Resources/Audio/OneOnGunman.acf")		//	acfファイル
#define ACB_FILE_BGM		("Resources/Audio/BGM.acb")		//	acbファイル
#define ACB_FILE_SE			("Resources/Audio/SE.acb")
#define AWB_FILE_BGM		("Resources/Audio/BGM.awb")		//	awbファイル

//	エラーをコールバックするための関数
void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
	UNREFERENCED_PARAMETER(parray);
	const CriChar8* errmsg;
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
	printf("%s\n", errmsg);
}

//	メモリを確保する関数
void* user_alloc_func(void* obj, CriUint32 size)
{
	UNREFERENCED_PARAMETER(obj);
	void* ptr;
	ptr = malloc(size);
	return ptr;
}

//	メモリを開放する関数
void user_free_func(void* obj, void* ptr)
{
	UNREFERENCED_PARAMETER(obj);
	free(ptr);
	return;
}


//	static変数の初期化
std::unique_ptr<AudioManager> AudioManager::s_instance = nullptr;

void AudioManager::Initialize()
{
	//	エラーコールバック関数の登録
	criErr_SetCallback(user_error_callback_func);
	//	メモリアロケーターの登録
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);
	//	ライブラリの初期化
	criAtomEx_Initialize_WASAPI(NULL, NULL, 0);
	// D-Basの作成
	m_dbas = criAtomDbas_Create(nullptr, nullptr, 0);
	//	ACFファイルの読み込みと登録
	criAtomEx_RegisterAcfFile(NULL, ACF_FILE, NULL, 0);
	//	DSP設定のアタッチ
	criAtomEx_AttachDspBusSetting(CRI_ONEONGUNMAN_ACF_DSPSETTING_DSPBUSSETTING_0, NULL, 0);
	//	ACBファイルを読み込み、ACBハンドルを作成
	m_acb_hn_BGM = criAtomExAcb_LoadAcbFile(NULL, ACB_FILE_BGM, NULL, AWB_FILE_BGM, NULL, 0);
	m_acb_hn_SE = criAtomExAcb_LoadAcbFile(NULL, ACB_FILE_SE, NULL, NULL, NULL, 0);
	//	ボイスプールの作成
	{
		CriAtomExStandardVoicePoolConfig standardVoicePoolConfig;
		criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standardVoicePoolConfig);
		standardVoicePoolConfig.player_config.streaming_flag = CRI_TRUE;	//	ストリーミング再生がデフォルトでfalseなので変更
		m_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standardVoicePoolConfig, NULL, 0);
	}
	//	プレーヤの作成
	m_playerBGM = criAtomExPlayer_Create(NULL, NULL, 0);
	m_playerSE = criAtomExPlayer_Create(NULL, NULL, 0);

	//	ボリュームを設定
	m_volumeBGM = SaveData::GetInstance()->GetVolumeBGM();
	m_volumeSE = SaveData::GetInstance()->GetVolumeSE();
	criAtomExPlayer_SetVolume(m_playerBGM, m_volumeBGM);
	criAtomExPlayer_SetVolume(m_playerSE, m_volumeSE);

	//	BGMにフェードアウトをつけるための設定
	criAtomExPlayer_AttachFader(m_playerBGM, NULL, NULL, 0);
	criAtomExPlayer_SetFadeOutTime(m_playerBGM, 1000);

	//	ボイスの優先度設定
	criAtomExPlayer_SetVoicePriority(m_playerBGM, 1);
	criAtomExPlayer_SetVoicePriority(m_playerSE, 0);

	criAtomExPlayer_UpdateAll(m_playerBGM);
	criAtomExPlayer_UpdateAll(m_playerSE);
}

void AudioManager::Update()
{
	//	サーバー処理の実行
	criAtomEx_ExecuteMain();
}

void AudioManager::Finalize()
{
	// DSPバス設定のデタッチ
	criAtomEx_DetachDspBusSetting();

	//	再生の停止
	criAtomExPlayer_SetFadeOutTime(m_playerBGM, 0);
	criAtomExPlayer_Stop(m_playerBGM);
	criAtomExPlayer_Stop(m_playerSE);
	
	//	Atomハンドルの破棄
	criAtomExPlayer_Destroy(m_playerBGM);
	criAtomExPlayer_Destroy(m_playerSE);

	//	ボイスプールの破棄
	criAtomExVoicePool_Free(m_pool);

	//	ACBハンドルの破棄
	criAtomExAcb_Release(m_acb_hn_BGM);
	criAtomExAcb_Release(m_acb_hn_SE);

	//	ACFの登録解除
	criAtomEx_UnregisterAcf();

	//	D-BASの破棄
	criAtomDbas_Destroy(m_dbas);

	//	ライブラリの終了
	criAtomEx_Finalize_WASAPI();
}

void AudioManager::StartBGM(CriAtomExCueId id)
{
	//	再生中かどうか確認
	if (criAtomExPlayer_GetStatus(m_playerBGM) == CRIATOMEXPLAYER_STATUS_PLAYING)
	{
		//	再生中なら新しくBGMを流し始めない(変更する場合は一度停止させる)
		return;
	}

	//	キューIDの指定
	criAtomExPlayer_SetCueId(m_playerBGM, m_acb_hn_BGM, id);
	//	再生の開始
	criAtomExPlayer_Start(m_playerBGM);
}

void AudioManager::StopBGM()
{	
	//	再生の停止
	criAtomExPlayer_Stop(m_playerBGM);
}

void AudioManager::PlaySE(CriAtomExCueId id)
{
	//	キューIDの指定
	criAtomExPlayer_SetCueId(m_playerSE, m_acb_hn_SE, id);
	//	再生の開始
	criAtomExPlayer_Start(m_playerSE);
}

void AudioManager::SetVolumeBGM(const float& volume)
{
	//	ボリュームを設定
	criAtomExPlayer_SetVolume(m_playerBGM, volume);
	criAtomExPlayer_UpdateAll(m_playerBGM);
	m_volumeBGM = volume;
}

void AudioManager::SetVolumeSE(const float& volume)
{
	//	ボリュームを設定
	criAtomExPlayer_SetVolume(m_playerSE, volume);
	criAtomExPlayer_UpdateAll(m_playerSE);
	m_volumeSE = volume;
}

void AudioManager::CreateInstance()
{	
	//	生成されていなければインスタンスを生成
	if (s_instance == nullptr)
	{
		s_instance.reset(new AudioManager());
	}
}

AudioManager* const AudioManager::GetInstance()
{
	return s_instance.get();
}

AudioManager::AudioManager() :
	m_playerBGM(),
	m_playerSE(),
	m_pool(),
	m_acb_hn_BGM(),
	m_acb_hn_SE(),
	m_dbas(),
	m_volumeBGM(0.f),
	m_volumeSE(0.f)
{
}