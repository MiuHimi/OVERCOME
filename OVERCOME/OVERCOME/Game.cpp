//////////////////////////////////////////////////////////////
// File.    Game.cpp
// Summary. GameClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "pch.h"
#include "Game.h"

#include "Utility/InputManager.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// デバッグ
//#if _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#endif

// externディレクトリ
extern void ExitGame();

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;


// Constructor
Game::Game()
{
	DX::DeviceResources::SingletonGetInstance().RegisterDeviceNotify(this);
}
Game::Game(bool isFullScreen)
{
	DX::DeviceResources::SingletonGetInstance().RegisterDeviceNotify(this);

	m_isFullScreen = isFullScreen;
}
// Destructor
Game::~Game()
{
	// サウンド終了
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Finalize();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	DX::DeviceResources::SingletonGetInstance().SetWindow(window, width, height);

	DX::DeviceResources::SingletonGetInstance().CreateDeviceResources();
    CreateDeviceDependentResources();

	DX::DeviceResources::SingletonGetInstance().CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	// サウンド再生
	ADX2Le* adx2le = ADX2Le::GetInstance();
	adx2le->Initialize(L"OVERCOME.acf");

	// スタートシーンの設定
	mp_sceneManager = std::make_unique<SceneManager>(SceneId::SCENE_LOGO, m_isFullScreen);

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

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
	// Escキーでゲーム終了する
	if (InputManager::SingletonGetInstance().GetKeyState().Escape)
	{
		bool closedGame = true;

		if (closedGame)
		{
			// シーンを削除
			mp_sceneManager->DeleteScene();
			// ゲームを終了する
			mp_sceneManager->SetGameState(false);
			ExitGame();
		}
	}

    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	if (mp_sceneManager->GetGameState())
	{
		// アクティブなシーンを更新
		mp_sceneManager->UpdateActiveScene(timer);
	}
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	if (mp_sceneManager->GetGameState())
	{
		// Don't try to render anything before the first Update.
		if (m_timer.GetFrameCount() == 0)
		{
			return;
		}

		Clear();

		DX::DeviceResources::SingletonGetInstance().PIXBeginEvent(L"Render");
		auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

		// ここから描画処理を記述する

		// アクティブなシーンを描画
		mp_sceneManager->RenderActiveScene();

		ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();

		// ここまで

		DX::DeviceResources::SingletonGetInstance().PIXEndEvent();

		// Show the new frame.
		DX::DeviceResources::SingletonGetInstance().Present();
	}
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	DX::DeviceResources::SingletonGetInstance().PIXBeginEvent(L"Clear");

    // Clear the views.
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();
	auto renderTarget = DX::DeviceResources::SingletonGetInstance().GetRenderTargetView();
	auto depthStencil = DX::DeviceResources::SingletonGetInstance().GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::Black);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
	auto viewport = DX::DeviceResources::SingletonGetInstance().GetScreenViewport();
    context->RSSetViewports(1, &viewport);

	OnDeviceLost();

	DX::DeviceResources::SingletonGetInstance().PIXEndEvent();
}

// CloseGame
// 閉じるボタンを押した際に実行される
void Game::CloseGame()
{
	// シーンを削除
	mp_sceneManager->DeleteScene();
	// ゲームを終了する
	mp_sceneManager->SetGameState(false);
}

// ChangeFullscreen
void Game::ChangeFullscreen(BOOL flag)
{
	DX::DeviceResources::SingletonGetInstance().GetSwapChain()->SetFullscreenState(flag, NULL);
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

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!DX::DeviceResources::SingletonGetInstance().WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}

#pragma endregion
