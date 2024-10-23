/*/
 *  �t�@�C����		�F	TitleScene.h
 *  �T�v			�F	�^�C�g���V�[���ɏ���������̂��W�߂��N���X
 *
 *  �쐬��			�F	�R�{���b
 *  �쐬��			�F	2023/06/12
/*/
#pragma once

#include "../Scene.h"

#include "Library/Camera.h"
#include "../UserInterface/UserInterface.h"
#include "TitleUIs/PleaseLeftClick.h"
#include "TitleUIs/TitleButtonManager.h"
#include "../PlayScene/StageObject/SkySphere.h"
#include "../UserInterface/GlitchEffect.h"

class TitleScene final : public Scene
{
public:
	TitleScene();
	~TitleScene() = default;

	void Initialize();
	void Update();
	void Render();
	void Finalize();

private:
	//	�J�����N���X
	std::unique_ptr<Camera> m_camera;

	std::unique_ptr<SkySphere> m_skySphere;

	//	�e�N�X�`��
	std::unique_ptr<UserInterface> m_grid;
	std::unique_ptr<GlitchEffect> m_logo;

	std::unique_ptr<PleaseLeftClick> m_pleaseLeftClick;
	std::unique_ptr<TitleButtonManager> m_buttonManager;

};