/*/
 *  �t�@�C����		�F	SkySphere.h
 *  �T�v			�F	�V����\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/26
/*/
#pragma once

class SkySphere
{
public:
	SkySphere();
	~SkySphere() = default;

	void Initialize();
	void Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	void Finalize();

private:
	//	���f��
	std::unique_ptr<DirectX::Model> m_model;

};
