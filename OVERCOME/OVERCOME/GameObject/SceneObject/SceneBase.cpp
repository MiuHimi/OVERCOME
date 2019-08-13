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
SceneBase::SceneBase(SceneManager* sceneManager, bool fullScreenFlag)
	: m_sceneManager(sceneManager)
{
	SetFullScreen(fullScreenFlag);
}
/// <summary>
/// デストラクタ
/// </summary>
SceneBase::~SceneBase()
{
}