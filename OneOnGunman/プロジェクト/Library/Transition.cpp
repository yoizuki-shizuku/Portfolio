/*/
 *  �t�@�C����		�F	Transition.cpp
 *  �T�v			�F	��ʑJ�ڂ��s��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/16
/*/
#include "pch.h"
#include "Transition.h"

#include "Easing.h"
#include "Library/UserResources.h"
#include "ReadData.h"

using namespace DirectX;

#define MAX_ALPHA	(1.f)
#define MIN_ALPHA	(0.f)

//	�t�F�[�h�ɓK������Lerp�֐�
#define LERP_FUNCTION	(Easing::InOutQuart)

//	�t�F�[�h�ɂ����鎞�Ԃ̃f�t�H���g�l
#define DEFAULT_FADE_TIME	(1.f)

//	�X�^�e�B�b�N�ϐ��̏�����
std::unique_ptr<Transition> Transition::s_instance = nullptr;

Transition::Transition() :
	m_screenW(0),
	m_screenH(0),
	m_fadeIn(false),
	m_fadeOut(false),
	m_beforeFadeIn(false),
	m_beforeFadeOut(false),
	m_alpha(0.f),
	m_fadeSeconds(0.f),
	m_processTimer(0.f)
{

}

void Transition::Initialize()
{
	//	�V�F�[�_�[�̍쐬
	CreateShader();

	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

	//	��ʂ̑傫����ݒ�
	RECT size = UserResources::GetInstance()->GetDeviceResources()->GetOutputSize();
	m_screenW = size.right;
	m_screenH = size.bottom;

	//	�����l�ݒ�
	m_alpha = MAX_ALPHA;
	m_fadeSeconds = DEFAULT_FADE_TIME;
	m_processTimer = 0.f;
}

void Transition::Update()
{
	float elapsedTime = float(UserResources::GetInstance()->GetStepTimer()->GetElapsedSeconds());

	m_beforeFadeIn = m_fadeIn;
	m_beforeFadeOut = m_fadeOut;

	FadeInUpdate(elapsedTime);
	FadeOutUpdate(elapsedTime);
}

void Transition::Render()
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();

	//	���_���
	VertexPositionTexture vertex[4] =
	{
		VertexPositionTexture(SimpleMath::Vector3(-1.f, 1.f,0.f) ,SimpleMath::Vector2(0.f, 1.f)),	//	����
		VertexPositionTexture(SimpleMath::Vector3( 1.f, 1.f,0.f) ,SimpleMath::Vector2(1.f, 1.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3( 1.f,-1.f,0.f) ,SimpleMath::Vector2(1.f, 0.f)),	//	�E��
		VertexPositionTexture(SimpleMath::Vector3(-1.f,-1.f,0.f) ,SimpleMath::Vector2(0.f, 0.f)),	//	����
	};

	//	�V�F�[�_�[�ɓn���o�b�t�@���쐬����
	ConstBuffer cbuff;
	cbuff.windowSize = SimpleMath::Vector2(static_cast<float>(m_screenW), static_cast<float>(m_screenH));
	cbuff.ratio = SimpleMath::Vector2(m_alpha, 0.f);

	//	�󂯓n���p�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//	�V�F�[�_�[�Ƀo�b�t�@��n��
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//�������`��w��
	auto states = UserResources::GetInstance()->GetCommonStates();
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// �J�����O�͍�����
	context->RSSetState(states->CullNone());

	//	�C���v�b�g���C�A�E�g�̓o�^
	context->IASetInputLayout(m_inputLayout.Get());

	//	�V�F�[�_�[���Z�b�g����
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//	�|���S����`��
	m_batch->Begin();
	m_batch->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_batch->End();

	//	�V�F�[�_�[�̓o�^���������Ă���
	context->VSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void Transition::Finalize()
{
}

void Transition::StartFadeIn()
{
	//	�t�F�[�h�A�E�g���s���ɏ����ł��Ȃ��悤�ɂ���
	if (m_fadeOut) return;

	m_processTimer = 0.f;
	m_fadeIn = true;
}

void Transition::StartFadeOut()
{
	//	�t�F�[�h�C�����s���ɏ����ł��Ȃ��悤�ɂ���
	if (m_fadeIn) return;

	m_processTimer = 0.f;
	m_fadeOut = true;
}

void Transition::CreateInstance()
{
	//	��������Ă��Ȃ���΃C���X�^���X�𐶐�
	if (s_instance == nullptr)
	{
		s_instance.reset(new Transition());
	}
}

//	�C���X�^���X�擾
Transition* const Transition::GetInstance()
{
	return s_instance.get();
}

void Transition::FadeInUpdate(float elapsedTime)
{
	//	�t�F�[�h�C�����s���łȂ���Ώ������Ȃ�
	if (!m_fadeIn) return;

	//	�^�C�}�[���X�V
	m_processTimer += elapsedTime;

	//	�������v�Z
	float t = m_processTimer / m_fadeSeconds;

	//	�������Ԃ��߂�����A�t�F�[�h�C�����I��
	if (m_processTimer > m_fadeSeconds)
	{
		m_alpha = MIN_ALPHA;
		m_fadeIn = false;
	}

	//	Lerp�œ����x��ύX
	m_alpha = Easing::Lerp(MAX_ALPHA, MIN_ALPHA, t, LERP_FUNCTION);
}

void Transition::FadeOutUpdate(float elapsedTime)
{
	//	�t�F�[�h�A�E�g���s���łȂ���Ώ������Ȃ�
	if (!m_fadeOut) return;

	//	�^�C�}�[���X�V
	m_processTimer += elapsedTime;

	//	�������v�Z
	float t = m_processTimer / m_fadeSeconds;

	//	�������Ԃ��߂�����A�t�F�[�h�C�����I��
	if (m_processTimer > m_fadeSeconds)
	{
		m_alpha = MIN_ALPHA;
		m_fadeOut = false;
	}

	//	Lerp�œ����x��ύX
	m_alpha = Easing::Lerp(MIN_ALPHA, MAX_ALPHA, t, LERP_FUNCTION);
}

void Transition::CreateShader()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�V�F�[�_�[�t�@�C���ǂݍ���
	std::vector<uint8_t> vsBlob, psBlob;

	vsBlob = DX::ReadData(L"Resources/CSO/Transition/TransitionVS.cso");
	psBlob = DX::ReadData(L"Resources/CSO/Transition/TransitionPS.cso");

	//	�e�V�F�[�_�[�̍쐬
	DX::ThrowIfFailed
	(
		device->CreateVertexShader(vsBlob.data(), vsBlob.size(), nullptr, m_vertexShader.ReleaseAndGetAddressOf())
	);

	DX::ThrowIfFailed
	(
		device->CreatePixelShader(psBlob.data(), psBlob.size(), nullptr, m_pixelShader.ReleaseAndGetAddressOf())
	);

	//	�C���v�b�g���C�A�E�g�̍쐬
	device->CreateInputLayout(DirectX::VertexPositionTexture::InputElements,
		DirectX::VertexPositionTexture::InputElementCount,
		vsBlob.data(), vsBlob.size(),
		m_inputLayout.GetAddressOf());

	//�V�F�[�_�[�Ƀf�[�^��n�����߂̃R���X�^���g�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
}
