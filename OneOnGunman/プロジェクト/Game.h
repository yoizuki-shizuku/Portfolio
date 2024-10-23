//
// Game.h
//

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"
#include "Library/UserResources.h"
#include "Library/Input.h"
#include "Scene/SceneManager.h"
#include "RenderTexture.h"


// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game final : public DX::IDeviceNotify
{
public:

    Game() noexcept(false);
    ~Game() = default;

    Game(Game&&) = default;
    Game& operator= (Game&&) = default;

    Game(Game const&) = delete;
    Game& operator= (Game const&) = delete;

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    void OnDeviceLost() override;
    void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnDisplayChange();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const noexcept;

private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    // Device resources.
    std::unique_ptr<DX::DeviceResources>    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

    // ���ʃX�e�[�g�ւ̃|�C���^
    std::unique_ptr<DirectX::CommonStates> m_states;

    //  ���[�U�[���\�[�X
    UserResources* m_userResources;

    //  �C���v�b�g
    Input* m_input;

    //  �V�[���}�l�[�W���[
    std::unique_ptr<SceneManager> m_sceneManager;

    //  �����_�[�e�N�X�`��
    std::unique_ptr<DX::RenderTexture> m_renderTexture;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

private:

    //  ��ʃ��[�h �iTRUE�F�t���X�N���[���j
    BOOL m_fullscreen;

public:
    //  ��ʃ��[�h��ύX����֐��iTRUE�F�t���X�N���[���j
    void SetFullscreenState(BOOL value)
    {
        m_fullscreen = value;
        m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
        if (value) m_deviceResources->CreateWindowSizeDependentResources();
    }

    /// <summary>
    /// �Q�[���̏I���t���O���擾
    /// </summary>
    /// <returns>true : �I��</returns>
    bool GetExitGame() { return m_sceneManager->GetExitGame(); }

    //  �I�����̏���
    void Finalize() { m_sceneManager->Finalize(); }
};
