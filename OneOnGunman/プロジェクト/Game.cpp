//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
    : m_fullscreen(FALSE)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>(
        DXGI_FORMAT_B8G8R8A8_UNORM,
        DXGI_FORMAT_D24_UNORM_S8_UINT
    );
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    //  ������
    m_input->Initialize();
    m_sceneManager->Initialize();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

    //  �C���v�b�g�N���X�̍X�V
    m_input->Update();

    //  �V�[���̍X�V
    m_sceneManager->Update();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    //  �V�[���̕`��
    m_sceneManager->Render();

    //  �`����ύX
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->OMSetRenderTargets(1, &renderTarget, nullptr);

    //  �X�v���C�g�o�b�`�ŉ�ʂ�`��
    m_spriteBatch->Begin(SpriteSortMode_Immediate,
        nullptr, nullptr, nullptr, nullptr, [=]
        {
            ID3D11PixelShader* pixel = m_sceneManager->GetPixelShader();
            ID3D11Buffer* buffer[1] = { m_sceneManager->GetConstBuffer() };

            if (pixel) context->PSSetShader(pixel, nullptr, 0);
            context->PSSetConstantBuffers(0, 1, buffer);
        });

    m_spriteBatch->Draw(m_renderTexture->GetShaderResourceView(), m_deviceResources->GetOutputSize());

    m_spriteBatch->End();

    //  ���\�[�X�̉��
    context->PSSetShader(nullptr, nullptr, 0);
    ID3D11ShaderResourceView* nullsrv[] = { nullptr };
    context->PSSetShaderResources(0, 1, nullsrv);

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_renderTexture->GetRenderTargetView();     //  �f�o�C�X���\�[�X���烌���_�[�e�N�X�`���ɕύX
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::WhiteSmoke);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);

    //  �t���X�N���[�������ׂ�
    BOOL fullscreen = FALSE;
    m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);
    //  �t���X�N���[������������Ă��܂������̏���
    if (m_fullscreen != fullscreen)
    {
        m_fullscreen = fullscreen;
        //  ResizeBuffers�֐����Ăяo��
        m_deviceResources->CreateWindowSizeDependentResources();
    }
}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: �f�t�H���g�̉�ʃT�C�Y (�ŏ��T�C�Y 320x200).
    width = 1280;
    height = 720;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    m_states = std::make_unique<CommonStates>(device);

    //  ���[�U�[���\�[�X�̍쐬
    UserResources::CreateInstance();
    m_userResources = UserResources::GetInstance();

    //  ���[�U�[���\�[�X�֊e��N���X�̃|�C���^��ݒ�
    m_userResources->SetDeviceResources(m_deviceResources.get());
    m_userResources->SetStepTimer(&m_timer);
    m_userResources->SetCommonStates(m_states.get());

    //  �C���v�b�g�N���X�̍쐬
    Input::CreateInstance();
    m_input = Input::GetInstace();

    //  �V�[���}�l�[�W���[�̍쐬
    m_sceneManager = std::make_unique<SceneManager>();

    //  �����_�[�e�N�X�`���̍쐬
    m_renderTexture = std::make_unique<DX::RenderTexture>(m_deviceResources->GetBackBufferFormat());
    //  �����_�[�e�N�X�`���Ƀf�o�C�X��n��
    m_renderTexture->SetDevice(device);

    //  �X�v���C�g�o�b�`
    auto context = m_deviceResources->GetD3DDeviceContext();
    m_spriteBatch = std::make_unique<SpriteBatch>(context);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

    //  ��ʃT�C�Y��n��
    auto size = m_deviceResources->GetOutputSize();
    m_renderTexture->SetWindow(size);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    //  �f�o�C�X�̉��
    m_renderTexture->ReleaseDevice();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
