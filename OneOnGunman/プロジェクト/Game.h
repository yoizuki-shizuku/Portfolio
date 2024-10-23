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

    // 共通ステートへのポインタ
    std::unique_ptr<DirectX::CommonStates> m_states;

    //  ユーザーリソース
    UserResources* m_userResources;

    //  インプット
    Input* m_input;

    //  シーンマネージャー
    std::unique_ptr<SceneManager> m_sceneManager;

    //  レンダーテクスチャ
    std::unique_ptr<DX::RenderTexture> m_renderTexture;
    std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

private:

    //  画面モード （TRUE：フルスクリーン）
    BOOL m_fullscreen;

public:
    //  画面モードを変更する関数（TRUE：フルスクリーン）
    void SetFullscreenState(BOOL value)
    {
        m_fullscreen = value;
        m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
        if (value) m_deviceResources->CreateWindowSizeDependentResources();
    }

    /// <summary>
    /// ゲームの終了フラグを取得
    /// </summary>
    /// <returns>true : 終了</returns>
    bool GetExitGame() { return m_sceneManager->GetExitGame(); }

    //  終了時の処理
    void Finalize() { m_sceneManager->Finalize(); }
};
