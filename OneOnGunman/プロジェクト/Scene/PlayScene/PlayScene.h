/*/
 *  �t�@�C����		�F	PlayScene.h
 *  �T�v			�F	�v���C�V�[���ɏ���������̂��W�߂��N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/12
/*/
#pragma once

#include "../Scene.h"

#include "../Observer/Subject.h"
#include "PlaySceneCamera.h"
#include "StageObject/SkySphere.h"
#include "StageObject/Floor.h"

#include "UI/BeginEvent.h"
#include "UI/FinishEvent.h"
#include "UI/Bar.h"
#include "UI/MoveUI.h"
#include "UI/MouseUI.h"
#include "UI/Pause.h"

#include "Player/Player.h"
#include "Player/PlayerBullet.h"

#include "Enemy/Enemy.h"
#include "Enemy/EnemyBulletManager.h"

class PlayScene final : public Scene
{
public:
	struct ConstBuffer
	{
		float ratio;
		float timer;
		DirectX::SimpleMath::Vector2 none;
	};

public:
	PlayScene(int stageIndex);
	~PlayScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	ID3D11PixelShader* GetPixelShader() const override { return m_postProcess.Get(); }
	ID3D11Buffer* GetConstBuffer() const override { return m_CBuffer.Get(); }

private:
	void CreateShader();
	//	���g���C���̃t���O������
	void RetryGame();

private:
	//	�J����
	std::unique_ptr<PlaySceneCamera> m_camera;
	//	�V��
	std::unique_ptr<SkySphere> m_skySphere;
	//	��
	std::unique_ptr<Floor> m_floor;

	//	�J�n�O���o�pUI
	std::unique_ptr<BeginEvent> m_ready;
	//	�I�������o�pUI
	std::unique_ptr<FinishEvent> m_finish;
	//	�v���C���[�̊e��N�[���^�C����\������o�[
	std::unique_ptr<Bar> m_bar;
	//	�v���C���[�̑���(�ړ�)��\������
	std::unique_ptr<MoveUI> m_moveUI;
	//	�v���C���[�̑���(�A�N�V����)��\������
	std::unique_ptr<MouseUI> m_mouseUI;
	//	�|�[�Y���
	std::unique_ptr<Pause> m_pause;

	//	�v���C���[
	std::unique_ptr<Player> m_player;
	//	�v���C���[�̒e
	std::unique_ptr<PlayerBullet> m_playerBullet;

	//	�G
	std::unique_ptr<Enemy> m_enemy;
	//	�G�̒e�Ǘ��N���X
	std::unique_ptr<EnemyBulletManager> m_bulletManager;

	//	�G��|�����ۂɒʒm�𑗂�
	std::unique_ptr<Subject> m_knockDownEnemy;
	//	�G��|�����t���O
	bool m_downEnemyFlag;
	//	�v���C���[�����ꂽ�ۂɒʒm�𑗂�
	std::unique_ptr<Subject> m_knockDownPlayer;
	//	�v���C���[�����ꂽ�t���O
	bool m_downPlayerFlag;
	//	�Q�[�������g���C����
	bool m_isRetry;

	//	�X�e�[�W�ԍ�
	int m_stageIndex;
	//	���Ԃ̐i�s���x
	float m_timeSpeed;
	//	�Q�[�����ł̌o�ߎ���
	float m_inGameTimer;


	//	�㏈���G�t�F�N�g
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_postProcess;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;

	//	�㏈���G�t�F�N�g�p�̃^�C�}�[
	float m_effectTimer;

};