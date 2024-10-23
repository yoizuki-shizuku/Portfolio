/*/
 *  �t�@�C����		�F	State.cpp
 *  �T�v			�F	�X�e�[�g�p�^�[���Ŋe�X�e�[�g���p������N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/09
/*/
#include "pch.h"
#include "State.h"

using namespace DirectX;

#include "Library/UserResources.h"

State::State() :
	m_model(nullptr),
	m_animSpeed(1.f)
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	�[�x�X�e���V���̍쐬 
	D3D11_DEPTH_STENCIL_DESC desc;

	// �`�掞�O���P�ɂ���
	desc.DepthEnable = TRUE;									// �[�x�e�X�g���s��
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;			// �[�x�o�b�t�@���X�V����
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;				// �[�x�l�ȉ��Ȃ�X�V����

	desc.StencilEnable = TRUE;									// �X�e���V���e�X�g���s��
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 0xff
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;	// 0xff

	// �\��
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;		// �O�Ȃ�
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;	// OK�@�P�{
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;		// NG�@�������Ȃ�
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// NG�@�������Ȃ�

	desc.BackFace = desc.FrontFace;	// ���ʂ�����

	DX::ThrowIfFailed(
		device->CreateDepthStencilState(&desc, m_depthStencilState.ReleaseAndGetAddressOf())
	);

}

void State::AnimeUpdate(float elapsedTime)
{
	//	�A�j���[�V�����̍X�V
	m_animSdk.Update(elapsedTime * m_animSpeed);
	size_t nbones = m_model->bones.size();
	m_animSdk.Apply(*m_model, nbones, m_bone.get());
}

void State::Render(SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix proj)
{
	//	�`��ɕK�v�ȕ����擾
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	//	�A�j���[�V������K�p�����`��
	size_t nbones = m_model->bones.size();

	m_model->DrawSkinned(
		context,
		*states,
		nbones,
		m_bone.get(),
		world,
		view,
		proj,
		false,
		[&]() {
			//	�������`��w��
			context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFF);
			//	�J�����O�͍�����
			context->RSSetState(states->CullClockwise());
			//	�[�x�o�b�t�@�̐ݒ�
			context->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
		}
	);
}

void State::SetModel(DirectX::Model* model, const wchar_t* fileName)
{
	//	�����o�ϐ��Ɋm��
	m_model = model;

	//	�A�j���[�V�����t�@�C���̓ǂݍ���
	size_t nbones = m_model->bones.size();
	m_animBone = ModelBone::MakeArray(nbones);
	m_model->CopyBoneTransformsTo(nbones, m_animBone.get());

	m_bone = ModelBone::MakeArray(nbones);
	m_model->CopyBoneTransformsFrom(nbones, m_animBone.get());

	for (size_t i = 0; i < nbones; ++i)
	{
		m_bone[i] = XMMatrixMultiply(m_model->invBindPoseMatrices[i], m_bone[i]);
	}

	//	�A�j���[�V�����̓ǂݍ���
	DX::ThrowIfFailed(
		m_animSdk.Load(fileName)
	);

	m_animSdk.Bind(*m_model);

	m_model->UpdateEffects([&](DirectX::IEffect* effect)
		{
			auto skin = dynamic_cast<DirectX::SkinnedEffect*>(effect);
			if (skin)
			{
				skin->SetPerPixelLighting(true);
			}
		}
	);
}
