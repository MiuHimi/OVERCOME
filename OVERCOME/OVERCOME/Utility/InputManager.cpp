//////////////////////////////////////////////////////////////
// File.    InputManager.cpp
// Summary. InputManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "InputManager.h"

// using�f�B���N�g��


/// <summary>
/// �R���X�g���N�^
/// </summary>
InputManager::InputManager()
{
	mp_keyboard = std::make_unique<DirectX::Keyboard>();
	mp_mouse = std::make_unique<DirectX::Mouse>();
}

/// <summary>
/// �}�E�X�̓��͏�Ԃ̎擾
/// </summary>
/// <returns>���͏��</returns>
DirectX::Mouse::State InputManager::GetMouseState()
{
	return mp_mouse->GetState();
}
/// <summary>
/// �L�[�̓��͏�Ԃ̎擾
/// </summary>
/// <returns>���͏��</returns>
DirectX::Keyboard::State InputManager::GetKeyState()
{
	return mp_keyboard->GetState();
}

/// <summary>
/// Tracker�̎擾
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
/// �}�E�X�����[�h�̕ύX
/// </summary>
void InputManager::SetMouseMode(DirectX::Mouse::Mode mode)
{
	mp_mouse->SetMode(mode);
}

/// <summary>
/// �X�V����
/// </summary>
void InputManager::Update()
{
	// �C���v�b�g�����X�V
	DirectX::Mouse::State mouseState = GetMouseState();
	m_mouseTracker.Update(mouseState);
	DirectX::Keyboard::State keyState = GetKeyState();
	m_keyTracker.Update(keyState);
}