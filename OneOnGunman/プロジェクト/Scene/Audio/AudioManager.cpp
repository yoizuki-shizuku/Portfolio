/*/
 *  �t�@�C����		�F	AudioManager.cpp
 *  �T�v			�F	���ʐݒ�ɑ����āABGM��SE���Đ�����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2024/01/28
/*/
#include "pch.h"
#include "AudioManager.h"

using namespace DirectX;

#include "Scene/SaveData/SaveData.h"

#define ACF_FILE			("Resources/Audio/OneOnGunman.acf")		//	acf�t�@�C��
#define ACB_FILE_BGM		("Resources/Audio/BGM.acb")		//	acb�t�@�C��
#define ACB_FILE_SE			("Resources/Audio/SE.acb")
#define AWB_FILE_BGM		("Resources/Audio/BGM.awb")		//	awb�t�@�C��

//	�G���[���R�[���o�b�N���邽�߂̊֐�
void user_error_callback_func(const CriChar8* errid, CriUint32 p1, CriUint32 p2, CriUint32* parray)
{
	UNREFERENCED_PARAMETER(parray);
	const CriChar8* errmsg;
	errmsg = criErr_ConvertIdToMessage(errid, p1, p2);
	printf("%s\n", errmsg);
}

//	���������m�ۂ���֐�
void* user_alloc_func(void* obj, CriUint32 size)
{
	UNREFERENCED_PARAMETER(obj);
	void* ptr;
	ptr = malloc(size);
	return ptr;
}

//	���������J������֐�
void user_free_func(void* obj, void* ptr)
{
	UNREFERENCED_PARAMETER(obj);
	free(ptr);
	return;
}


//	static�ϐ��̏�����
std::unique_ptr<AudioManager> AudioManager::s_instance = nullptr;

void AudioManager::Initialize()
{
	//	�G���[�R�[���o�b�N�֐��̓o�^
	criErr_SetCallback(user_error_callback_func);
	//	�������A���P�[�^�[�̓o�^
	criAtomEx_SetUserAllocator(user_alloc_func, user_free_func, NULL);
	//	���C�u�����̏�����
	criAtomEx_Initialize_WASAPI(NULL, NULL, 0);
	// D-Bas�̍쐬
	m_dbas = criAtomDbas_Create(nullptr, nullptr, 0);
	//	ACF�t�@�C���̓ǂݍ��݂Ɠo�^
	criAtomEx_RegisterAcfFile(NULL, ACF_FILE, NULL, 0);
	//	DSP�ݒ�̃A�^�b�`
	criAtomEx_AttachDspBusSetting(CRI_ONEONGUNMAN_ACF_DSPSETTING_DSPBUSSETTING_0, NULL, 0);
	//	ACB�t�@�C����ǂݍ��݁AACB�n���h�����쐬
	m_acb_hn_BGM = criAtomExAcb_LoadAcbFile(NULL, ACB_FILE_BGM, NULL, AWB_FILE_BGM, NULL, 0);
	m_acb_hn_SE = criAtomExAcb_LoadAcbFile(NULL, ACB_FILE_SE, NULL, NULL, NULL, 0);
	//	�{�C�X�v�[���̍쐬
	{
		CriAtomExStandardVoicePoolConfig standardVoicePoolConfig;
		criAtomExVoicePool_SetDefaultConfigForStandardVoicePool(&standardVoicePoolConfig);
		standardVoicePoolConfig.player_config.streaming_flag = CRI_TRUE;	//	�X�g���[�~���O�Đ����f�t�H���g��false�Ȃ̂ŕύX
		m_pool = criAtomExVoicePool_AllocateStandardVoicePool(&standardVoicePoolConfig, NULL, 0);
	}
	//	�v���[���̍쐬
	m_playerBGM = criAtomExPlayer_Create(NULL, NULL, 0);
	m_playerSE = criAtomExPlayer_Create(NULL, NULL, 0);

	//	�{�����[����ݒ�
	m_volumeBGM = SaveData::GetInstance()->GetVolumeBGM();
	m_volumeSE = SaveData::GetInstance()->GetVolumeSE();
	criAtomExPlayer_SetVolume(m_playerBGM, m_volumeBGM);
	criAtomExPlayer_SetVolume(m_playerSE, m_volumeSE);

	//	BGM�Ƀt�F�[�h�A�E�g�����邽�߂̐ݒ�
	criAtomExPlayer_AttachFader(m_playerBGM, NULL, NULL, 0);
	criAtomExPlayer_SetFadeOutTime(m_playerBGM, 1000);

	//	�{�C�X�̗D��x�ݒ�
	criAtomExPlayer_SetVoicePriority(m_playerBGM, 1);
	criAtomExPlayer_SetVoicePriority(m_playerSE, 0);

	criAtomExPlayer_UpdateAll(m_playerBGM);
	criAtomExPlayer_UpdateAll(m_playerSE);
}

void AudioManager::Update()
{
	//	�T�[�o�[�����̎��s
	criAtomEx_ExecuteMain();
}

void AudioManager::Finalize()
{
	// DSP�o�X�ݒ�̃f�^�b�`
	criAtomEx_DetachDspBusSetting();

	//	�Đ��̒�~
	criAtomExPlayer_SetFadeOutTime(m_playerBGM, 0);
	criAtomExPlayer_Stop(m_playerBGM);
	criAtomExPlayer_Stop(m_playerSE);
	
	//	Atom�n���h���̔j��
	criAtomExPlayer_Destroy(m_playerBGM);
	criAtomExPlayer_Destroy(m_playerSE);

	//	�{�C�X�v�[���̔j��
	criAtomExVoicePool_Free(m_pool);

	//	ACB�n���h���̔j��
	criAtomExAcb_Release(m_acb_hn_BGM);
	criAtomExAcb_Release(m_acb_hn_SE);

	//	ACF�̓o�^����
	criAtomEx_UnregisterAcf();

	//	D-BAS�̔j��
	criAtomDbas_Destroy(m_dbas);

	//	���C�u�����̏I��
	criAtomEx_Finalize_WASAPI();
}

void AudioManager::StartBGM(CriAtomExCueId id)
{
	//	�Đ������ǂ����m�F
	if (criAtomExPlayer_GetStatus(m_playerBGM) == CRIATOMEXPLAYER_STATUS_PLAYING)
	{
		//	�Đ����Ȃ�V����BGM�𗬂��n�߂Ȃ�(�ύX����ꍇ�͈�x��~������)
		return;
	}

	//	�L���[ID�̎w��
	criAtomExPlayer_SetCueId(m_playerBGM, m_acb_hn_BGM, id);
	//	�Đ��̊J�n
	criAtomExPlayer_Start(m_playerBGM);
}

void AudioManager::StopBGM()
{	
	//	�Đ��̒�~
	criAtomExPlayer_Stop(m_playerBGM);
}

void AudioManager::PlaySE(CriAtomExCueId id)
{
	//	�L���[ID�̎w��
	criAtomExPlayer_SetCueId(m_playerSE, m_acb_hn_SE, id);
	//	�Đ��̊J�n
	criAtomExPlayer_Start(m_playerSE);
}

void AudioManager::SetVolumeBGM(const float& volume)
{
	//	�{�����[����ݒ�
	criAtomExPlayer_SetVolume(m_playerBGM, volume);
	criAtomExPlayer_UpdateAll(m_playerBGM);
	m_volumeBGM = volume;
}

void AudioManager::SetVolumeSE(const float& volume)
{
	//	�{�����[����ݒ�
	criAtomExPlayer_SetVolume(m_playerSE, volume);
	criAtomExPlayer_UpdateAll(m_playerSE);
	m_volumeSE = volume;
}

void AudioManager::CreateInstance()
{	
	//	��������Ă��Ȃ���΃C���X�^���X�𐶐�
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