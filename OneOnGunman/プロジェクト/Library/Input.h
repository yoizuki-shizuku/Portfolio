/*/
 *  �t�@�C����		�F	Input.h
 *  �T�v			�F	�X�e�[�g�g���b�J�[���V���O���g����
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/05
/*/
#pragma once

class Input
{
public:
	//	�f�X�g���N�^
	~Input() = default;

	void Initialize();
	void Update();

	//	�L�[�{�[�h�g���b�J�[
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardTracker() const { return m_kbTracker.get(); }
	//	�}�E�X�g���b�J�[
	DirectX::Mouse::ButtonStateTracker* GetMouseTracker() const { return m_msTracker.get(); }

public:
	//	�C���X�^���X����
	static void CreateInstance();

	//	�C���X�^���X�擾
	static Input* const GetInstace();

private:
	//	�R���X�g���N�^
	Input();

	//	����A�R�s�[�̃R���X�g���N�^���폜
	void operator=(const Input& object) = delete;
	Input(const Input& object) = delete;

private:
	//	�C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<Input> s_instance;

	//	�L�[�{�[�h�g���b�J�[
	std::unique_ptr<DirectX::Keyboard::KeyboardStateTracker> m_kbTracker;

	//	�}�E�X�g���b�J�[
	std::unique_ptr<DirectX::Mouse::ButtonStateTracker> m_msTracker;

};