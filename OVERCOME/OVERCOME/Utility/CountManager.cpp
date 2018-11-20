//////////////////////////////////////////////////////////////
// File.    CountManager.cpp
// Summary. CountManagerClass(Frame Count)
// Date.    2018/11/20
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "CountManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
CountManager::CountManager()
	: m_count(0),
	  m_countFlag(false)
{
}
/// <summary>
/// デストラクタ
/// </summary>
CountManager::~CountManager()
{
}
/// <summary>
/// 更新
/// </summary>
void CountManager::Update()
{

}

/// <summary>
/// カウントを開始
/// </summary>
void CountManager::CountStart()
{
	m_countFlag = true;
}
/// <summary>
/// カウントしていく
/// </summary>
void CountManager::Counting()
{
	if (m_countFlag)m_count++;
}
/// <summary>
/// カウントをリセット
/// </summary>
void CountManager::CountReset()
{
	m_count = 0;
}
/// <summary>
/// カウント数(フレーム数)を取得
/// </summary>
/// <returns>フレーム数</returns>
int CountManager::GetCount()
{
	return m_count;
}
/// <summary>
/// FPSを取得
/// </summary>
/// <returns>FPS</returns>
int CountManager::GetFPS()
{
	return m_fps;
}
