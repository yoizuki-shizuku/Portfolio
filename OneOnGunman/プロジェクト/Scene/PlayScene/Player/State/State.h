/*/
 *  �t�@�C����		�F	State.h
 *  �T�v			�F	�X�e�[�g�p�^�[���Ŋe�X�e�[�g���p������N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/08/09
/*/
#pragma once

#include "Animation.h"

class State
{
public:
	State();
	virtual ~State() = default;

	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void AnimeUpdate(float elapsedTime) final;
	virtual void Render(DirectX::SimpleMath::Matrix world,
						DirectX::SimpleMath::Matrix view,
						DirectX::SimpleMath::Matrix proj);
	virtual void Finalize() = 0;

	//	���f���̃A�j���[�V�����ݒ�i���ʕ������֐����j
	virtual void SetModel(DirectX::Model* model, const wchar_t* fileName) final;

	//	�A�j���[�V�����̊J�n���Ԃ�ݒ�
	virtual void SetAnimStartTime(const double& startTime) final { m_animSdk.SetStartTime(startTime); }
	//	�A�j���[�V�����̏I�����Ԃ�ݒ�
	virtual void SetAnimEndTime(const double& endTime)     final { m_animSdk.SetEndTime(endTime); }
	//	�A�j���[�V���������[�v���邩��ݒ�
	virtual void SetAnimLoop(const bool& loop)             final { m_animSdk.SetLoop(loop); }
	//	�A�j���[�V�������͂��߂���Đ�
	virtual void RestartAnimetion()                        final { m_animSdk.RestartAnim(); }
	//	�A�j���[�V�����̑��x��ݒ�
	virtual void SetAnimSpeed(const float& speed)		   final { m_animSpeed = speed; }

private:
	//	���f��
	DirectX::Model* m_model;
	//	SDK�A�j���[�V����
	DX::AnimationSDKMESH m_animSdk;
	//	�X�P���g��
	DirectX::ModelBone::TransformArray m_bone;
	//	�A�j���[�V�����X�P���g��
	DirectX::ModelBone::TransformArray m_animBone;
	//	�X�e���V���}�X�N�p
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	//	�A�j���[�V�����̑��x
	float m_animSpeed;

};