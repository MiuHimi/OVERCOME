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


Game::Game()
{
    //m_deviceResources = std::make_unique<DX::DeviceResources>();
	DX::DeviceResources::SingletonGetInstance().RegisterDeviceNotify(this);
    //m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	// カメラオブジェクトの作成
	//mp_camera = std::make_unique<MyCamera>();

	// デバッグカメラの作成
	//m_debugCamera = std::make_unique<DebugCamera>(width, height);

    //m_deviceResources->SetWindow(window, width, height);
	DX::DeviceResources::SingletonGetInstance().SetWindow(window, width, height);

    //m_deviceResources->CreateDeviceResources();
	DX::DeviceResources::SingletonGetInstance().CreateDeviceResources();
    CreateDeviceDependentResources();

    //m_deviceResources->CreateWindowSizeDependentResources();
	DX::DeviceResources::SingletonGetInstance().CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	mp_sceneManager = std::make_unique<SceneManager>(this, SceneId::SCENE_TITLE);

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

	//m_deviceResources->PIXBeginEvent(L"Render");
	DX::DeviceResources::SingletonGetInstance().PIXBeginEvent(L"Render");
	//auto context = m_deviceResources->GetD3DDeviceContext();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	// ビュー行列の作成
	//m_view = Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), Vector3::Up);
	//m_view = m_debugCamera->GetCameraMatrix();

	// グリッドの床の描画
	//m_gridFloor->Render(context, m_view, m_projection);

	// ここから描画処理を記述する

	/*// 回転用変数
	static float angle = 0.0f;
	angle += 0.3f;
	// サイン波が変動するための値
	static float wave;
	wave += 0.01f;
	float sinWave = sin(wave + (1.0f*2.0f)) * 2.0f;*/

	// アクティブなシーンを描画
	m_sprite = new SpriteBatch(context);
	mp_sceneManager->RenderActiveScene(m_sprite, this);
	delete m_sprite;

	//ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();
	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);

	// ここまで

    //m_deviceResources->PIXEndEvent();
	DX::DeviceResources::SingletonGetInstance().PIXEndEvent();

    // Show the new frame.
    //m_deviceResources->Present();
	DX::DeviceResources::SingletonGetInstance().Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    //m_deviceResources->PIXBeginEvent(L"Clear");
	DX::DeviceResources::SingletonGetInstance().PIXBeginEvent(L"Clear");

    // Clear the views.
    //auto context = m_deviceResources->GetD3DDeviceContext();
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();
    //auto renderTarget = m_deviceResources->GetRenderTargetView();
	auto renderTarget = DX::DeviceResources::SingletonGetInstance().GetRenderTargetView();
    //auto depthStencil = m_deviceResources->GetDepthStencilView();
	auto depthStencil = DX::DeviceResources::SingletonGetInstance().GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::DarkBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    //auto viewport = m_deviceResources->GetScreenViewport();
	auto viewport = DX::DeviceResources::SingletonGetInstance().GetScreenViewport();
    context->RSSetViewports(1, &viewport);

	OnDeviceLost();

    //m_deviceResources->PIXEndEvent();
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
    if (/*!m_deviceResources->WindowSizeChanged(width, height)*/!DX::DeviceResources::SingletonGetInstance().WindowSizeChanged(width, height))
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
    //ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();
	//ID3D11DeviceContext* context =  m_deviceResources->GetD3DDeviceContext();
	ID3D11DeviceContext* context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;

	// スプライトバッチの作成
	m_sprites = std::make_unique<SpriteBatch>(context);
	//m_sprite = new SpriteBatch(context);

	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

	// グリッドの床の作成
	//m_gridFloor = std::make_unique<GridFloor>(device, context, m_states.get(), 10.0f, 10);

	// モデルのテクスチャの読み込み
	EffectFactory fx(device);
	fx.SetDirectory(L"Resources\\Models");
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

	// ウインドウサイズからアスペクト比を算出する
	//RECT size = m_deviceResources->GetOutputSize();
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

	// デバッグカメラにウインドウのサイズ変更を伝える
	//m_debugCamera->SetWindowSize(size.right, size.bottom);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

	// コモンステートの解放
	m_states.reset();

	// スプライトバッチの解放
	m_sprites.reset();

	// スプライトフォントの解放
	m_font.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}

DirectX::CommonStates* Game::GetState()
{
	//ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();
	// コモンステートの作成
	m_states = std::make_unique<CommonStates>(device);

	return m_states.get();
}
#pragma endregion
