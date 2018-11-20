//////////////////////////////////////////////////////////////
// File.    InputManager.cpp
// Summary. InputManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "InputManager.h"

// usingディレクトリ


/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager()
{
	mp_keyboard = std::make_unique<DirectX::Keyboard>();
	mp_mouse = std::make_unique<DirectX::Mouse>();
}

/// <summary>
/// マウスの入力状態の取得
/// </summary>
/// <returns>入力状態</returns>
DirectX::Mouse::State InputManager::GetMouseState()
{
	return mp_mouse->GetState();
}
/// <summary>
/// キーの入力状態の取得
/// </summary>
/// <returns>入力状態</returns>
DirectX::Keyboard::State InputManager::GetKeyState()
{
	return mp_keyboard->GetState();
}

/// <summary>
/// Trackerの取得
/// </summary>
/// <returns>Tracker</returns>
DirectX::Mouse::ButtonStateTracker InputManager::GetTracker()
{
	return m_mouseTracker;
}
DirectX::Keyboard::KeyboardStateTracker InputManager::GetKeyTracker()
{
	return m_keyTracker;
}

/// <summary>
/// マウス軸モードの変更
/// </summary>
void InputManager::SetMouseMode(DirectX::Mouse::Mode mode)
{
	mp_mouse->SetMode(mode);
}

/// <summary>
/// 更新処理
/// </summary>
void InputManager::Update()
{
	// インプット情報を更新
	DirectX::Mouse::State mouseState = GetMouseState();
	m_mouseTracker.Update(mouseState);
	DirectX::Keyboard::State keyState = GetKeyState();
	m_keyTracker.Update(keyState);
}