//
// Game.h
//

#pragma once

#include "pch.h"

#include "DeviceResources.h"
#include "StepTimer.h"

#include "DebugCamera.h"
//#include "GridFloor.h"

//#include "GameObject/Camera.h"
#include "GameObject/MyCamera.h"

//#include "GameObject\SceneObject\ScenePlay.h"

#include "GameObject/SceneObject/SceneManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class SceneManager;
class MyCamera;
class Game : public DX::IDeviceNotify
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

	// Getter
	//---
	// デバイスを取得する
	ID3D11Device* GetDevice() { return m_deviceResources->GetD3DDevice(); }
	// コンテクストを取得する
	ID3D11DeviceContext* GetContext() { return m_deviceResources->GetD3DDeviceContext(); }
	// コモンステートを取得する
	DirectX::CommonStates* GetState()/* { return m_states.get(); }*/;

	// ビュー行列を取得する
	const DirectX::SimpleMath::Matrix& GetView() { return m_view; }
	// 射影行列を取得する
	const DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }
	//---//

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

	// コモンステート
	std::unique_ptr<DirectX::CommonStates> m_states;

	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_sprites;
	DirectX::SpriteBatch* m_sprite;

	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_font;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	// デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;

	// グリッドの床
	//std::unique_ptr<GridFloor> m_gridFloor;
	
	// カメラオブジェクト
	//Camera m_camera;
	std::unique_ptr<MyCamera> mp_camera;

	// シーン管理オブジェクト
	std::unique_ptr<SceneManager> mp_sceneManager;

	std::unique_ptr<DirectX::Model> m_modelLattice;   // 格子柄床モデルハンドル
};