/*/
 *  �t�@�C����		�F	EnemyBulletManager.cpp
 *  �T�v			�F	�G�̒e���ꊇ�Ǘ�����N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/25
/*/
#include "pch.h"
#include "EnemyBulletManager.h"

using namespace DirectX;

#include "ReadData.h"
#include "Library/UserResources.h"

EnemyBulletManager::EnemyBulletManager()
{
}

void EnemyBulletManager::Initialize()
{
	m_bullets.clear();

	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, psBlob;
	vsBlob = DX::ReadData(L"Resources/CSO/BulletEffect/EnemyBulletVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/BulletEffect/EnemyBulletPS.cso");

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(
		DirectX::VertexPositionNormalTexture::InputElements,
		DirectX::VertexPositionNormalTexture::InputElementCount,
		vsBlob.data(),
		vsBlob.size(),
		m_inputLayout.GetAddressOf()
	);

	m_landingEffect = std::make_unique<LandingEffect>();
	m_landingEffect->Initialize();
}

void EnemyBulletManager::Update(float elapsedTime)
{
	//	�e�e��Update�֐�����������
	for (std::vector<std::unique_ptr<EnemyBullet>>::iterator it = m_bullets.begin(); it != m_bullets.end();)
	{
		//	�߂�l��false�Ȃ�e����������
		if (it->get()->Update(elapsedTime))
		{
			++it;
			continue;
		}

		it->get()->Finalize();
		it->reset();
		it = m_bullets.erase(it);
	}

	//	�G�t�F�N�g�̍X�V����
	m_landingEffect->Update(elapsedTime);
}

void EnemyBulletManager::Render(SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	�`��
	for (auto& it : m_bullets)
	{
		it->Render(view, proj);
	}

	m_landingEffect->Render(view, proj);
}

void EnemyBulletManager::Finalize()
{
	auto it = m_bullets.begin();

	//	�㏈��
	while (m_bullets.size() != 0)
	{
		it->get()->Finalize();
		it->reset();
		it = m_bullets.erase(it);
	}

	m_landingEffect->Finalize();
}

void EnemyBulletManager::AddBullet(std::unique_ptr<EnemyBullet> bullet)
{
	//	�������������s���Ă���z��ɒǉ�����
	bullet->Initialize(m_inputLayout.Get(), m_vertexShader.Get(), m_pixelShader.Get());
	m_bullets.emplace_back(std::move(bullet));
}

bool EnemyBulletManager::CheckHitBullet(Collider* col, Collider* dodge)
{
	//	�W���X�g����̔���p�t���O
	bool justDodge = false;

	for (auto& it : m_bullets)
	{
		//	�R���C�_�[�N���X�̎擾
		Collider* bCol = it->GetCollider();

		//	�W���X�g����̔�������
		//	�v���C���[�̓����蔻��͂��̋��E���Ɋ܂܂�Ă���
		//	�܂�����ɓ������Ă��Ȃ���΃J�v�Z���Ƃ̔���͂��Ȃ�
		if (!dodge->CheckHit(bCol)) continue;

		//	1�ł��W���X�g����̋��E���ɓ������Ă���
		justDodge = true;

		//	�����蔻������
		bool hit = bCol->CheckHit(col);

		//	��ł��������Ă���΁A���͏������Ȃ�
		if (hit)
		{
			//	���e�G�t�F�N�g��\��
			SimpleMath::Vector3 hitPos = it->GetPosition();
			SimpleMath::Vector3 bulletColor = it->GetColor();
			m_landingEffect->StartEffect(hitPos, bulletColor);

			return true;
		}
	}

	//	���̃t���[���ŋ��E�����ڐG���Ă������ǂ���
	dodge->SetIsHit(justDodge);

	//	�ǂ̒e�Ƃ��������Ă��Ȃ�
	return false;
}

void EnemyBulletManager::RetryGame()
{
	m_bullets.clear();
	m_landingEffect->StopEffect();
}
