/*/
 *  �t�@�C����		�F	Loading.h
 *  �T�v			�F	���[�h��ʂ�\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/27
/*/
#pragma once

#include "../Scene/UserInterface/GlitchEffect.h"

class Loading
{
public:
	Loading();
	~Loading() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

private:
	//	�v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> m_batch;
	//	�x�[�V�b�N�G�t�F�N�g
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	//	�C���v�b�g���C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	//	NowLoading�摜
	std::unique_ptr<GlitchEffect> m_texture;

};