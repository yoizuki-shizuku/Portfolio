/*/
 *  �t�@�C����		�F	StageSelectScene.h
 *  �T�v			�F	�X�e�[�W�I��������V�[��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/09/04
/*/
#pragma once

#include "../Scene.h"

#include "Library/Camera.h"
#include "Scene/UserInterface/UserInterface.h"
#include "Scene/UserInterface/Animator.h"
#include "Scene/UserInterface/Button.h"
#include "Model/HologramManager.h"
#include "Model/InfinitePlane.h"
#include "Information.h"

class StageSelectScene : public Scene
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	x�̂�
	};

public:
	StageSelectScene(int stageIndex);
	~StageSelectScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	//	�J����
	std::unique_ptr<Camera> m_camera;
	//	�X�e�[�W�Z���N�g�̕\��
	std::unique_ptr<UserInterface> m_stageSelect;
	//	�G�̃z���O����
	std::unique_ptr<HologramManager> m_hologram;
	//	�w�i�p�̖���(�Ɍ�����)����
	std::unique_ptr<InfinitePlane> m_plane;
	//	�X�e�[�W���
	std::unique_ptr<Information> m_infomation;
	//	�X�^�[�g�{�^��
	std::unique_ptr<UserInterface> m_start;
	std::unique_ptr<Button> m_startButton;
	std::unique_ptr<Animator> m_startAnimation;
	//	�^�C�g���{�^��
	std::unique_ptr<UserInterface> m_title;
	std::unique_ptr<Button> m_titleButton;
	std::unique_ptr<Animator> m_titleAnimation;

	//	�n�߂ɕ\������X�e�[�W
	int m_stageIndex;

	//	�㏈���G�t�F�N�g
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	�^�C�}�[
	float m_timer;

};