/*/
 *  �t�@�C����		�F	Transition.h
 *  �T�v			�F	��ʑJ�ڂ��s��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/16
/*/
#pragma once

class Transition
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector2 windowSize;
		DirectX::SimpleMath::Vector2 ratio;
	};

public:
	~Transition() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

	/// <summary>
	/// �t�F�[�h�ɂ����鎞�Ԃ�ݒ�
	/// </summary>
	/// <param name="seconds">�t�F�[�h�ɂ����鎞��</param>
	void SetFadeSpeed(float seconds) { m_fadeSeconds = seconds; }

	/// <summary>
	/// �t�F�[�h�ɂ����鎞�Ԃ��擾
	/// </summary>
	/// <returns>�P�� : �b(s)</returns>
	const float GetFadeSecond() { return m_fadeSeconds; }

	//	�t�F�[�h�C�����J�n����
	void StartFadeIn();
	//	�t�F�[�h�A�E�g���J�n����
	void StartFadeOut();

	/// <summary>
	/// �t�F�[�h�C���̏��������`�F�b�N����
	/// </summary>
	/// <returns>true : ������</returns>
	bool ProcessCheckFadeIn() { return m_fadeIn; }
	/// <summary>
	/// �t�F�[�h�A�E�g�̏��������`�F�b�N����
	/// </summary>
	/// <returns>true : ������</returns>
	bool ProcessCheckFadeOut() { return m_fadeOut; }
	/// <summary>
	/// �t�F�[�h�C���̏I���������m
	/// </summary>
	/// <returns>true : �I������1f</returns>
	bool FinishFadeIn() { return !m_fadeIn && m_beforeFadeIn; }
	/// <summary>
	/// �t�F�[�h�A�E�g�̏I���������m
	/// </summary>
	/// <returns>true : �I������1f</returns>
	bool FinishFadeOut() { return !m_fadeOut && m_beforeFadeOut; }

public:
	//	�C���X�^���X����
	static void CreateInstance();
	//	�C���X�^���X�擾
	static Transition* const GetInstance();

	//	����A�R�s�[�̃R���X�g���N�^���폜
	void operator=(const Transition& object) = delete;
	Transition(const Transition& object) = delete;

private:
	Transition();

	//	�t�F�[�h�C���̏���
	void FadeInUpdate(float elapsedTime);
	//	�t�F�[�h�A�E�g�̏���
	void FadeOutUpdate(float elapsedTime);
	//	�V�F�[�_�[�̍쐬
	void CreateShader();

private:
	//	�C���X�^���X�ւ̃|�C���^
	static std::unique_ptr<Transition> s_instance;

	//	��ʂ̃T�C�Y
	int m_screenW, m_screenH;

	//	���s���̃t�F�[�h����
	std::atomic<bool> m_fadeIn, m_fadeOut;
	//	�I�������m�p
	bool m_beforeFadeIn, m_beforeFadeOut;

	//	�����x
	float m_alpha;
	//	�t�F�[�h�ɂ�����b��
	float m_fadeSeconds;
	//	�����̎��Ԃ��v��
	float m_processTimer;

	//	�V�F�[�_�[�ɏ���n�����߂̃o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

};