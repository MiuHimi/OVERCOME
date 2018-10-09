//////////////////////////////////////////////////////////////
// File.    Game.h
// Summary. GameClass
// Date.    2018/09/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "pch.h"

#include "DeviceResources.h"
#include "StepTimer.h"

//#include "DebugCamera.h"
//#include "GridFloor.h"
//#include "GameObject/Camera.h"
//#include "GameObject/MyCamera.h"

#include "GameObject/SceneObject/SceneManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class SceneManager;
class Game : public DX::IDeviceNotify
{
// メンバー変数
public:

private:
	//std::unique_ptr<DX::DeviceResources>   m_deviceResources;     // Device resources.

	DX::StepTimer                            m_timer;               // Rendering loop timer.

	std::unique_ptr<DirectX::CommonStates>   m_states;              // コモンステート
	std::unique_ptr<DirectX::SpriteBatch>    m_sprites;             // スプライトバッチ
	DirectX::SpriteBatch*                    m_sprite;              // スプライトバッチ
	std::unique_ptr<DirectX::SpriteFont>     m_font;                // スプライトフォント

	DirectX::SimpleMath::Matrix              m_world;               // ワールド行列
	DirectX::SimpleMath::Matrix              m_view;                // ビュー行列
	DirectX::SimpleMath::Matrix              m_projection;          // 射影行列
	
	//std::unique_ptr<DebugCamera>           m_debugCamera;         // デバッグカメラ
	//std::unique_ptr<GridFloor>             m_gridFloor;           // グリッドの床
	//Camera                                 m_camera;              // カメラオブジェクト
	//std::unique_ptr<MyCamera>              mp_camera;             // カメラオブジェクト

	std::unique_ptr<SceneManager>            mp_sceneManager;       // シーン管理オブジェクト

// メンバー関数(関数、Getter、Setter)
public:
	// Constructor
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
	//ID3D11Device* GetDevice() { return m_deviceResources->GetD3DDevice(); }
	// コンテキストを取得する
	//ID3D11DeviceContext* GetContext() { return m_deviceResources->GetD3DDeviceContext(); }
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

};