/*/
 *  �t�@�C����		�F	Hologram.h
 *  �T�v			�F	�X�e�[�W�I�����̓G�̌����ڂ��z���O�����ŕ\��
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/16
/*/
#pragma once

class Hologram
{
public:
	//�f�[�^�󂯓n���p�R���X�^���g�o�b�t�@(���M��)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Vector4 Time;	//	x�̂�
	};

public:
	Hologram();
	~Hologram() = default;

	void Initialize(const wchar_t* model, const wchar_t* tex);
	void Update();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void SetScale(float scale) { m_scale = scale; }

private:
	void CreateShader();

private:
	//	���f��
	std::unique_ptr<DirectX::Model> m_model;
	//	�萔�o�b�t�@�[
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_CBuffer;
	//	�s�N�Z���V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	//	���_�V�F�[�_�[
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�摜
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//	�\���A�j���[�V�����p�̃��f���̑傫��
	float m_scale;

	//	�A�j���[�V�����p�^�C�}�[
	float m_timer;

};
