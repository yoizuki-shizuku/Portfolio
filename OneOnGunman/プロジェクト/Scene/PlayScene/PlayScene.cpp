/*/
 *  �t�@�C����		�F	PlayScene.cpp
 *  �T�v			�F	�v���C�V�[���ɏ���������̂��W�߂��N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/12
/*/
#include "pch.h"
#include "PlayScene.h"
#include "Scene/StageSelectScene/StageSelectScene.h"
#include "Scene/ResultScene/ResultScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Library/Useful.h"
#include "ReadData.h"
#include "Scene/SaveData/SaveData.h"
#include "../Audio/AudioManager.h"

#include "Enemy/Enemies/Guardian.h"
#include "Enemy/Enemies/Collector.h"
#include "Enemy/Enemies/Invader.h"

//	�X���[���̎��Ԕ{��
#define SLOW_TIME_SPEED		(0.3f)

PlayScene::PlayScene(int stageIndex) :
	Scene(),
	m_downEnemyFlag(false),
	m_downPlayerFlag(false),
	m_isRetry(false),
	m_stageIndex(stageIndex),
	m_timeSpeed(1.f),
	m_inGameTimer(0.f),
	m_effectTimer(0.f)
{
}

void PlayScene::Initialize()
{
	//	����
	m_camera        = std::make_unique<PlaySceneCamera>();
	m_skySphere     = std::make_unique<SkySphere>();
	m_floor         = std::make_unique<Floor>();

	m_ready         = std::make_unique<BeginEvent>();
	m_finish        = std::make_unique<FinishEvent>();
	m_bar           = std::make_unique<Bar>();
	m_moveUI        = std::make_unique<MoveUI>();
	m_mouseUI       = std::make_unique<MouseUI>();
	m_pause         = std::make_unique<Pause>();

	m_player        = std::make_unique<Player>();
	m_playerBullet  = std::make_unique<PlayerBullet>();

	m_bulletManager = std::make_unique<EnemyBulletManager>();

	//	stageIndex�ɉ����ēG��ύX
	     if (m_stageIndex == 0) m_enemy = std::make_unique<Guardian>();
	else if (m_stageIndex == 1) m_enemy = std::make_unique<Collector>();
	else if (m_stageIndex == 2) m_enemy = std::make_unique<Invader>();

	//	������
	m_camera->Initialize();
	m_skySphere->Initialize();
	m_floor->Initialize();

	m_ready->Initialize();
	m_finish->Initialize();
	m_bar->Initialize();
	m_moveUI->Initialize();
	m_mouseUI->Initialize();
	m_pause->Initialize();

	m_player->Initialize(m_playerBullet.get(), &m_timeSpeed);
	m_playerBullet->Initialize();
	m_player->Update(0.016f);	//	�A�j���[�V�������1/60�b���i�߂Ă���(�\�������ꂽ�̂ő΍�)

	m_enemy->Initialize(m_player.get(), m_bulletManager.get());
	m_bulletManager->Initialize();

	//	�G�̌��j�C�x���g�̏������A�ݒ�
	m_knockDownEnemy = std::make_unique<Subject>();
	m_knockDownEnemy->Attach(m_camera->GetClearObserver());
	m_knockDownEnemy->Attach(m_finish->GetClearObserver());
	m_knockDownEnemy->Attach(m_bar->GetFinishObserver());
	m_knockDownEnemy->Attach(m_enemy->GetObserver());
	m_knockDownEnemy->Attach(m_moveUI->GetObserver());
	m_knockDownEnemy->Attach(m_mouseUI->GetObserver());

	//	�v���C���[�̎��s�C�x���g�̏������A�ݒ�
	m_knockDownPlayer = std::make_unique<Subject>();
	m_knockDownPlayer->Attach(m_camera->GetFailureObserver());
	m_knockDownPlayer->Attach(m_finish->GetFailureObserver());
	m_knockDownPlayer->Attach(m_bar->GetFinishObserver());
	m_knockDownPlayer->Attach(m_player->GetFailureObserver());
	m_knockDownPlayer->Attach(m_moveUI->GetObserver());
	m_knockDownPlayer->Attach(m_mouseUI->GetObserver());

	//	�v���C�񐔂̍X�V
	SaveData::GetInstance()->AddTryCount(m_stageIndex);

	CreateShader();

	//	BGM�̍Đ�
	AudioManager::GetInstance()->StartBGM(CRI_BGM_CONFRONTATION);
}

void PlayScene::Update()
{
	m_camera->SetPlayerPosition(m_player->GetPosition());
	m_camera->SetEnemyPosition(m_enemy->GetPosition());
	m_camera->Update();

	//	Ready�̃A�j���[�V�������I�������珈�����n�߂�
	if (!m_ready->Update()) return;

	//	�o�ߎ��Ԃ��擾
	float elapsedTime = static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	�i�s���x��K�p
	float gameTime = elapsedTime * m_timeSpeed;

	//	�|�[�Y�����ǂ���
	Pause::Process isPause = Pause::Process::MAINTAIN;

	//	�~�X�܂��͐������o���ȊO�ōs������
	if (!m_downEnemyFlag && !m_downPlayerFlag)
	{
		//	���Ԃ��v��
		m_inGameTimer += gameTime;

		//	�t���O�̍X�V
		isPause = m_pause->Update();
	}

	//	���o���I��������X�e�[�W�Z���N�g�֖߂�
	FinishEvent::Process result = m_finish->Update();
	Transition* transition = Transition::GetInstance();

	//	�t�F�[�h�A�E�g���I�������t���O���������A�Q�[�����ĊJ
	if (transition->FinishFadeOut() && m_isRetry)
	{
		RetryGame();
		transition->StartFadeIn();
		m_isRetry = false;
		return;
	}

	//	�Q�[�����͂��߂����蒼��
	if (result == FinishEvent::Process::RETRY || isPause == Pause::Process::RESTART && !m_isRetry)
	{
		transition->StartFadeOut();
		m_isRetry = true;
	}
	//	�X�e�[�W�Z���N�g�֖߂�
	else if (result == FinishEvent::Process::SELECT || isPause == Pause::Process::SELECT)
	{
		ChangeScene<StageSelectScene>(m_stageIndex);
	}
	//	�N���A
	else if (result == FinishEvent::Process::CLEAR)
	{
		ChangeScene<ResultScene>(m_inGameTimer, m_player->GetShotNum(), m_stageIndex);
	}

	//	�|�[�Y��ԂȂ珈���𒆒f
	if (isPause != Pause::Process::MAINTAIN) return;

	m_player->Update(gameTime);
	m_player->SetFront(m_camera->GetFront());
	m_bar->Update(m_player->GetShotRatio(), m_player->GetDodgeRatio());
	m_moveUI->Update();
	m_mouseUI->Update();

	m_enemy->Update(gameTime);

	m_playerBullet->Update(gameTime);
	m_bulletManager->Update(gameTime);

	//	�G�ɒe�����������猂�j�C�x���g��ʒm
	if (m_enemy->GetCollider()->CheckHit(m_playerBullet->GetCollider()) && !m_downPlayerFlag)
	{
		m_knockDownEnemy->Notify();
		m_timeSpeed = SLOW_TIME_SPEED;
		m_downEnemyFlag = true;
	}

	//	�v���C���[�����ꂽ��C�x���g��ʒm
	if (!m_downEnemyFlag)
	{
		if (m_bulletManager->CheckHitBullet(m_player->GetCollider(), m_player->GetJustDodge()))
		{
			m_knockDownPlayer->Notify();
			m_timeSpeed = SLOW_TIME_SPEED;
			m_downPlayerFlag = true;
		}
	}

	//	�v���C���[�����ꂽ���ʂ����m�N���ɂ��Ă���
	if (m_downPlayerFlag)
	{
		m_effectTimer += elapsedTime;
		m_timeSpeed = SLOW_TIME_SPEED;
	}

	//	�N���A��͎��Ԃ̐i�݂��Œ�
	if (m_downEnemyFlag) m_timeSpeed = SLOW_TIME_SPEED;
}

void PlayScene::Render()
{
	//	view��projection���擾
	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	//	�`��
	m_skySphere->Render(view, proj);
	m_floor->Render(view, proj);

	m_player->Render(view, proj);
	m_enemy->Render(view, proj);
	m_playerBullet->Render(view, proj);
	m_bulletManager->Render(view, proj);

	m_bar->Render();
	m_moveUI->Render();
	m_mouseUI->Render(m_player->GetShotRatio(), m_player->GetDodgeRatio());
	m_ready->Render();
	m_finish->Render();
	m_pause->Render();

	//	��ʌ���
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	float ratio = Useful::Ratio01(m_effectTimer, 2.f);

	ConstBuffer cbuff;
	cbuff.ratio = ratio;
	cbuff.timer = m_effectTimer;
	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
}

void PlayScene::Finalize()
{
	m_skySphere->Finalize();
	m_camera->Finalize();
	m_floor->Finalize();
	m_ready->Finalize();
	m_player->Finalize();
	m_playerBullet->Finalize();
	m_enemy->Finalize();
	m_bulletManager->Finalize();

	AudioManager::GetInstance()->StopBGM();
}

void PlayScene::CreateShader()
{
	//  �V�F�[�_�[�ǂݍ���
	std::vector<uint8_t> psBlob;
	psBlob = DX::ReadData(L"Resources/CSO/PostProcess/Monochrome.cso");
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();
	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_postProcess.ReleaseAndGetAddressOf())
	);

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

void PlayScene::RetryGame()
{
	m_downEnemyFlag = false;
	m_downPlayerFlag = false;
	m_timeSpeed = 1.f;
	m_inGameTimer = 0.f;
	m_effectTimer = 0.f;
	m_knockDownEnemy->Reset();
	m_knockDownPlayer->Reset();

	m_camera->RetryGame();
	m_ready->RetryGame();
	m_finish->RetryGame();
	m_bar->RetryGame();
	m_moveUI->RetryGame();
	m_mouseUI->RetryGame();
	m_pause->RetryGame();
	m_player->RetryGame();
	m_playerBullet->RetryGame();
	m_enemy->RetryGame();
	m_bulletManager->RetryGame();

	SaveData::GetInstance()->AddTryCount(m_stageIndex);
}
