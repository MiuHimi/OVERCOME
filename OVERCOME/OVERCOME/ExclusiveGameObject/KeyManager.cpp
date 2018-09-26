//////////////////////////////////////////////////////////////
// File.    KeyManager.cpp
// Summary. KeyManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "KeyManager.h"
//#include <windows.h>
//#include <memory.h>

// usingディレクトリ


/// <summary>
/// コンストラクタ
/// </summary>
KeyManager::KeyManager()
{
}
/// <summary>
/// デストラクタ
/// </summary>
KeyManager::~KeyManager()
{
}

/// <summary>
/// キー入力情報をセット
/// </summary>
/// <param name="key">キー名前</param>
void KeyManager::SetTrigger(int key)
{
	//m_aTrigCount[key]++;
}

/// <summary>
/// キー入力情報をリセット、毎フレーム呼び出し
/// </summary>
void KeyManager::AllClear()
{
	//ZeroMemory(m_aTrigCount, sizeof(m_aTrigCount));
}

/// <summary>
/// キー入力情報を取得
/// </summary>
/// <param name="key">キー名前</param>
/// <returns>キー入力情報</returns>
bool KeyManager::IsTrigger(int key)
{
	//return (m_aTrigCount[key] > 0);
	return false;
}
