/*/
 *  �t�@�C����		�F	ResultScene.h
 *  �T�v			�F	�X�e�[�W���N���A������Ƀ��U���g��\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/16
/*/
#pragma once

#include "../Scene.h"
#include "Scene/UserInterface/Numeral.h"
#include "Scene/UserInterface/UserInterface.h"
#include "UIs/ResultButtonManager.h"
#include "UIs/ClearTime.h"
#include "UIs/ShotCount.h"
#include "Library/Camera.h"
#include "../StageSelectScene/Model/InfinitePlane.h"

class ResultScene final : public Scene
{
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	x�̂�
	};

public:
	ResultScene(float time, int shotCount, int stageIndex);
	~ResultScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	//	�J����
	std::unique_ptr<Camera> m_camera;
	//	��������
	std::unique_ptr<InfinitePlane> m_plane;
	//	�u���U���g�v�̕\��
	std::unique_ptr<UserInterface> m_result;
	//	�N���A�^�C���̕\��
	std::unique_ptr<ClearTime> m_clearTimeDisplay;
	//	�ˌ��񐔂̕\��
	std::unique_ptr<ShotCount> m_shotCountDisplay;
	//	�{�^��
	std::unique_ptr<ResultButtonManager> m_buttonManager;

	//	�X�e�[�W�̃N���A����
	float m_clearTime;
	//	�ˌ���
	int m_shotCount;
	//	�X�e�[�W�ԍ�
	int m_stageIndex;

	//	�㏈���G�t�F�N�g
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	�^�C�}�[
	float m_timer;

};