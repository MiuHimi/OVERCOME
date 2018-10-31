//////////////////////////////////////////////////////////////
// File.    Game.cpp
// Summary. GameClass
// Date.    2018/09/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "pch.h"
#include "Game.h"

#include "math.h"

#include "Utility/MatrixManager.h"

#include "GameObject/SceneObject/ScenePlay.h"

// デバッグ
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

// externディレクトリ
extern void ExitGame();

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

//DirectX::SimpleMath::Matrix MatrixManager::m_projection;


// Constructor
Game::Game()
{
	DX::DeviceResources::SingletonGetInstance().RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	// カメラオブジェクトの作成
	//mp_camera = std::make_unique<MyCamera>();

	// デバッグカメラの作成
	//m_debugCamera = std::make_unique<DebugCamera>(width, height);

	DX::DeviceResources::SingletonGetInstance().SetWindow(window, width, height);

	DX::DeviceResources::SingletonGetInstance().CreateDeviceResources();
    CreateDeviceDependentResources();

	DX::DeviceResources::SingletonGetInstance().CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();


	// スタートシーンの設定
	mp_sceneManager = std::make_unique<SceneManager>(this, SceneId::SCENE_SELECTSTAGE);

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
    float elapsedTime = float(timer.GetElapsedSeconds());

    // TODO: Add your game logic here.
    elapsedTime;

	// デバッグカメラの更新
	//m_debugCamera->Update();

	// カメラの位置の設定
	//mp_camera.SetPositionTarget(Vector3(0.0f, 4.0f, 8.0f), Vector3(0.0f, 0.0f, 0.0f));

	// カメラの更新
	//mp_camera->Update(timer, /*mp_player->GetPlayer())*/ScenePlay::GetPlayer()->GetPlayer());

	// アクティブなシーンを更新
	mp_sceneManager->UpdateActiveScene(timer, this);
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

	DX::DeviceResources::SingletonGetInstance().PIXBeginEvent(L"Render");
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// ビュー行列の作成
	//m_view = Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), Vector3::Up);
	//m_view = m_debugCamera->GetCameraMatrix();

	// グリッドの床の描画
	//m_gridFloor->Render(context, m_view, m_projection);

	// ここから描画処理を記述する

	// アクティブなシーンを描画
	mp_sceneManager->RenderActiveScene();

	ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();

	// ここまで

	DX::DeviceResources::SingletonGetInstance().PIXEndEvent();

    // Show the new frame.
	DX::DeviceResources::SingletonGetInstance().Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
	DX::DeviceResources::SingletonGetInstance().PIXBeginEvent(L"Clear");

    // Clear the views.
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();
	auto renderTarget = DX::DeviceResources::SingletonGetInstance().GetRenderTargetView();
	auto depthStencil = DX::DeviceResources::SingletonGetInstance().GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::DarkBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
	auto viewport = DX::DeviceResources::SingletonGetInstance().GetScreenViewport();
    context->RSSetViewports(1, &viewport);

	OnDeviceLost();

	DX::DeviceResources::SingletonGetInstance().PIXEndEvent();
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

	// グリッドの床の作成
	//m_gridFloor = std::make_unique<GridFloor>(device, context, m_states.get(), 10.0f, 10);
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);

	// 画角を設定
	float fovAngleY = XMConvertToRadians(45.0f);

	// 射影行列を作成する
	m_projection = Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// 射影行列を設定
	MatrixManager::SingletonGetInstance().SetProjection(m_projection);

	// デバッグカメラにウインドウのサイズ変更を伝える
	//m_debugCamera->SetWindowSize(size.right, size.bottom);
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
