/*/
 *  �t�@�C����		�F	ResultScene.cpp
 *  �T�v			�F	�X�e�[�W���N���A������Ƀ��U���g��\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/16
/*/
#include "pch.h"
#include "ResultScene.h"

using namespace DirectX;

#include "Library/UserResources.h"
#include "Library/Input.h"
#include "../PlayScene/PlayScene.h"
#include "../StageSelectScene/StageSelectScene.h"
#include "ReadData.h"
#include "Scene/SaveData/SaveData.h"
#include "../Audio/AudioManager.h"

ResultScene::ResultScene(float time, int shotCount, int stageIndex) :
	Scene(),
	m_clearTime(time),
	m_shotCount(shotCount),
	m_stageIndex(stageIndex),
	m_timer(0.f)
{
}

void ResultScene::Initialize()
{
	//	�J�����̐����Ə����ݒ�
	m_camera = std::make_unique<Camera>();
	m_camera->SetEye(SimpleMath::Vector3(0.f, 1.f, -5.f));
	m_camera->SetTarget(SimpleMath::Vector3(0.f, 1.f, 0.f));
	m_camera->Initialize();

	m_plane = std::make_unique<InfinitePlane>();

	//	��ʃT�C�Y�擾
	RECT  rect = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	float width = static_cast<float>(rect.right);
	float height = static_cast<float>(rect.bottom);

	//	�N���X�̐����Ə�����
	m_result = std::make_unique<UserInterface>();
	m_result->LoadTexture(L"Resources/Texture/Result/Result.png");
	m_result->SetAnchor(Anchor::MIDDLE_CENTER);
	m_result->SetPosition(SimpleMath::Vector2(width * 0.5f, height * 0.15f));
	m_result->SetScale(SimpleMath::Vector2::One * (width / 1280.f) * 0.8f);

	m_clearTimeDisplay = std::make_unique<ClearTime>();
	m_clearTimeDisplay->Initialize(m_clearTime);

	m_shotCountDisplay = std::make_unique<ShotCount>();
	m_shotCountDisplay->Initialize(m_shotCount);

	m_buttonManager = std::make_unique<ResultButtonManager>();
	m_buttonManager->Initialize(m_stageIndex);

	//  �V�F�[�_�[�ǂݍ���
	std::vector<uint8_t> psBlob;
	psBlob = DX::ReadData(L"Resources/CSO/PostProcess/CathodeRayTube.cso");
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

	//	�v���C�f�[�^�X�V
	SaveData* savedata = SaveData::GetInstance();
	savedata->SetTime(m_stageIndex, static_cast<int>(m_clearTime));
	if (m_shotCount <= 1) savedata->SuccsesOneShot(m_stageIndex);

	AudioManager::GetInstance()->StartBGM(CRI_BGM_IYA_IYA_UNCONTROLLABLE);
}

void ResultScene::Update()
{
	m_plane->Update();
	m_clearTimeDisplay->Update();
	m_shotCountDisplay->Update();

	//	UI�̃{�^�����͂����m
	ResultButtonManager::Process button = m_buttonManager->Update();

	m_timer += static_cast<float>(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	//	�܂����͂���Ă��Ȃ���Ώ������Ȃ�
	if (button == ResultButtonManager::Process::MAINTAIN) return;

	//	���͂�����ΑΉ����鑀����s��
	switch (button)
	{
	case ResultButtonManager::Process::NEXT:
		AudioManager::GetInstance()->StopBGM();
		ChangeScene<PlayScene>(m_stageIndex + 1);
		break;

	case ResultButtonManager::Process::RETRY:
		AudioManager::GetInstance()->StopBGM();
		ChangeScene<PlayScene>(m_stageIndex);
		break;

	case ResultButtonManager::Process::SELECT:
		ChangeScene<StageSelectScene>(m_stageIndex);
		break;

	default:
		break;
	}
}

void ResultScene::Render()
{
	//	�J�����̃r���[���X�V
	m_camera->CreateView();
	SimpleMath::Matrix view = m_camera->GetView();
	SimpleMath::Matrix proj = m_camera->GetProjection();

	m_plane->Render(view, proj);
	m_result->Render();
	m_clearTimeDisplay->Render();
	m_shotCountDisplay->Render();
	m_buttonManager->Render();

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	ConstBuffer cbuff;
	cbuff.Time = SimpleMath::Vector4(m_timer, 0.f, 0.f, 0.f);
	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
}

void ResultScene::Finalize()
{
	m_buttonManager->Finalize();
}