//////////////////////////////////////////////////////////////
// File.    InputManager.h
// Summary. InputManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

#include <Keyboard.h>
#include <Mouse.h>

#include "SingletonBase.h"

class InputManager : public SingletonBase<InputManager>
{
// メンバー変数
private:
	std::unique_ptr<DirectX::Keyboard> mp_keyboard;        // キー入力情報
	std::unique_ptr<DirectX::Mouse> mp_mouse;              // マウス入力情報
	

	DirectX::Mouse::ButtonStateTracker m_mouseTracker;          // マウストラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyTracker;  // キートラッカー

// メンバー関数(関数、Getter、Setter)
private:
	// フレンド関数
	friend class SingletonBase<InputManager>;

	// コンストラクタ
	InputManager();

public:

	// 更新
	void Update();

	// マウスの状態取得
	DirectX::Mouse::State GetMouseState();
	// キーボードの状態取得
	DirectX::Keyboard::State GetKeyState();
	// Trackerの取得
	DirectX::Mouse::ButtonStateTracker GetTracker();
	DirectX::Keyboard::KeyboardStateTracker GetKeyTracker();

	// マウス軸モードの変更
	void SetMouseMode(DirectX::Mouse::Mode mode);

	// マウスカーソル表示/非表示
	void SetVisibleMouseCursor(bool cursorState);

	// マウス座標の取得
	int GetMousePosX() { return GetMouseState().x; }
	int GetMousePosY() { return GetMouseState().y; }
};