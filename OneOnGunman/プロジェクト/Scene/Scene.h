/*/
 *  �t�@�C����		�F	Scene.h
 *  �T�v			�F	�V�[���N���X�ŋ��L�̊֐�������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/09
/*/
#pragma once

#include <utility>
#include "Library/Transition.h"

class Scene
{
public:
	Scene();
	virtual ~Scene() = default;

	virtual void Initialize() = 0;
	virtual void Update()     = 0;
	virtual void Render()     = 0;
	virtual void Finalize()   = 0;

	/// <summary>
	/// Update�O�ɂ��Ă������ʏ���
	/// </summary>
	virtual void CommonUpdate() final;
	/// <summary>
	/// �Q�[���̏I���t���O���擾
	/// </summary>
	/// <returns>true : �I��</returns>
	virtual bool GetExitGame() const final { return m_exitGame; }
	//	�V�[���̕ύX�^�C�~���O���擾
	virtual bool GetChangeScene() const final { return m_isChange; }
	//	�ύX��̃V�[���N���X�̃|�C���^���擾
	virtual Scene* GetNextScene() final { return m_nextScene; }

	//	�Q�[�����I������
	virtual void ExitGame() final { m_exitGame = true; }

	//	�V�[����ύX����
	template <class T, class... Args>
	inline void ChangeScene(Args&&... arg)
	{
		//	Scene���p�������N���X���ǂ������ׂ�
		static_assert(std::is_base_of_v<Scene, T>);

		//	�J�ڎ��s���͕�������s����Ă������ɂȂ�悤�ɂ���
		if (m_isFadeOut || m_isFadeIn || m_isChange) return;

		//	�K�v�ȏ����Z�b�g���A�J�ڂ��J�n����
		m_nextScene = new T(std::forward<Args>(arg)...);
		Transition::GetInstance()->StartFadeOut();
		m_isFadeOut = true;
	}

	//	�|�X�g�v���Z�X�p�̃s�N�Z���V�F�[�_�[��n��
	virtual ID3D11PixelShader* GetPixelShader() const { return nullptr; }
	//	�|�X�g�v���Z�X�p�̒萔�o�b�t�@�[��n��
	virtual ID3D11Buffer* GetConstBuffer() const { return nullptr; }

private:
	//	�Q�[���̏I���t���O
	bool m_exitGame;

	//	�V�[���̕ύX�^�C�~���O
	bool m_isChange;

	//	�V�[���ύX�̃t�F�[�h�C�������s��
	bool m_isFadeIn;

	//	�V�[���ύX�̃t�F�[�h�A�E�g�����s��
	bool m_isFadeOut;

	//	�ύX��̃V�[���N���X�̃|�C���^
	Scene* m_nextScene;

};