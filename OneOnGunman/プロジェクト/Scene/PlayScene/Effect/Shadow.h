/*/
 *  �t�@�C����		�F	Shadow.h
 *  �T�v			�F	�|���S�����g���ȈՓI�ȉe��\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/10/06
/*/
#pragma once

class Shadow
{
public:
	Shadow();
	~Shadow() = default;

	void Update(DirectX::SimpleMath::Vector3 position);
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

	void SetSize(DirectX::SimpleMath::Vector2 size) { m_size = size; }

private:
	//	�|���S����\������v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch;
	//	�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	//	�`�悷��ʒu
	DirectX::SimpleMath::Vector3 m_position;
	//	�T�C�Y
	DirectX::SimpleMath::Vector2 m_size;

};
