//////////////////////////////////////////////////////////////
// File.    KeyManager.cpp
// Summary. KeyManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "KeyManager.h"
//#include <windows.h>
//#include <memory.h>

// using�f�B���N�g��


/// <summary>
/// �R���X�g���N�^
/// </summary>
KeyManager::KeyManager()
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
KeyManager::~KeyManager()
{
}

/// <summary>
/// �L�[���͏����Z�b�g
/// </summary>
/// <param name="key">�L�[���O</param>
void KeyManager::SetTrigger(int key)
{
	//m_aTrigCount[key]++;
}

/// <summary>
/// �L�[���͏������Z�b�g�A���t���[���Ăяo��
/// </summary>
void KeyManager::AllClear()
{
	//ZeroMemory(m_aTrigCount, sizeof(m_aTrigCount));
}

/// <summary>
/// �L�[���͏����擾
/// </summary>
/// <param name="key">�L�[���O</param>
/// <returns>�L�[���͏��</returns>
bool KeyManager::IsTrigger(int key)
{
	//return (m_aTrigCount[key] > 0);
	return false;
}
