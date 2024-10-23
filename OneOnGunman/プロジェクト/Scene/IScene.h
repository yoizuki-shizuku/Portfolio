/*/
 *  �t�@�C����		�F	IScene.h
 *  �T�v			�F	�V�[���N���X�̃C���^�[�t�F�[�X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/09
/*/
#pragma once

class Scene;

__interface IScene
{
	void Initialize();
	void Update();
	void Render();
	void Finalize();

	//	���ʂ�Update
	void CommonUpdate();
	//	�V�[����ύX
	void ChangeScene(std::function<Scene* ()> func);
	//	�Q�[�����I��
	void ExitGame();
	/// <summary>
	/// �Q�[���̏I���t���O���擾
	/// </summary>
	/// <returns>true : �I��</returns>
	const bool GetExitGame();
	//	�V�[���̕ύX�^�C�~���O���擾
	const bool GetChangeScene();
	//	�ύX��̃V�[�����擾
	const std::function<Scene* ()> GetNextScene();

};
