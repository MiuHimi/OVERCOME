//////////////////////////////////////////////////////////////
// File.    Game.h
// Summary. GameClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "pch.h"

#include "Utility/DeviceResources.h"
#include "Utility/StepTimer.h"

#include "GameObject/SceneObject/SceneManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class SceneManager;
class Game : public DX::IDeviceNotify
{
// メンバー変数
public:

private:
	DX::StepTimer                            m_timer;               // Rendering loop timer.
	
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

private:
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

};