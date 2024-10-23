/*/
 *  �t�@�C����		�F	Floor.cpp
 *  �T�v			�F	����\������
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/07/26
/*/
#include "pch.h"
#include "Floor.h"

#include "Library/UserResources.h"

using namespace DirectX;

//	1�ӂ̒���
#define SIDE_SCALE	(10.f)

Floor::Floor()
{
}

void Floor::Initialize()
{
	auto device = UserResources::GetInstance()->GetDeviceResources()->GetD3DDevice();

	//	cmo���f���̐���
	std::unique_ptr<EffectFactory> fx = std::make_unique<EffectFactory>(device);
	fx->SetDirectory(L"Resources/CMO/");
	m_model = Model::CreateFromCMO(device, L"Resources/CMO/Helipad.cmo", *fx);
}

void Floor::Render(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = UserResources::GetInstance()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = UserResources::GetInstance()->GetCommonStates();

	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	world *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.f, -241.f, 0.f));
	world *= SimpleMath::Matrix::CreateScale(0.01f);

	m_model->Draw(context, *states, world, view, proj);
}

void Floor::Finalize()
{
}
