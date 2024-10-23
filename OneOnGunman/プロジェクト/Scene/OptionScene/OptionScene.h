/*/
 *  �t�@�C����		�F	OptionScene.h
 *  �T�v			�F	�e��I�v�V�������s���V�[��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2024/02/05
/*/
#pragma once

#include "Scene/Scene.h"
#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include "Library/Camera.h"
#include "../StageSelectScene/Model/InfinitePlane.h"
#include "UI/Volume.h"

class OptionScene final : public Scene
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	x�̂�
	};

public:
	OptionScene();
	~OptionScene() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;
	void Finalize() override;

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	//	�J����
	std::unique_ptr<Camera> m_camera;
	//	��������
	std::unique_ptr<InfinitePlane> m_plane;
	//	�㏈���G�t�F�N�g
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	�uOption�v�̕\��
	std::unique_ptr<UserInterface> m_option;
	//	�^�C�g���֖߂�{�^��
	std::unique_ptr<UserInterface> m_title;
	std::unique_ptr<Animator> m_titleAnimation;
	std::unique_ptr<Button> m_titleButton;
	
	//	BGM�̉��ʒ���
	std::unique_ptr<UserInterface> m_bgm;
	std::unique_ptr<Volume> m_bgmVolume;

	//	SE�̉��ʒ���
	std::unique_ptr<UserInterface> m_se;
	std::unique_ptr<Volume> m_seVolume;

	//	(�f�o�b�O)�Z�[�u�f�[�^�̃��Z�b�g
	std::unique_ptr<UserInterface> m_resetData;
	std::unique_ptr<Button> m_resetDataButton;

	//	�^�C�}�[
	float m_timer;

};