//////////////////////////////////////////////////////////////
// File.    SceneBase.cpp
// Summary. SceneBassClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "SceneBase.h"
#include "SceneManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneBase::SceneBase(SceneManager* sceneManager)
	: m_sceneManager(sceneManager)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneBase::~SceneBase()
{
}