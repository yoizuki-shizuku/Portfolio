/*/
 *  �t�@�C����		�F	Floor.h
 *  �T�v			�F	����\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/26
/*/
#pragma once

class Floor
{
public:
	Floor();
	~Floor() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	//	���f��
	std::unique_ptr<DirectX::Model> m_model;

};