//////////////////////////////////////////////////////////////
// File.    SceneBase.h
// Summary. SceneBaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../StepTimer.h"

#include "../../Game.h"

#include "../../Utility/InputManager.h"

class SceneManager;
class SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

protected:
	SceneManager* m_sceneManager;   // 他クラスへのポインタ

private:

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneBase(SceneManager* sceneManager);
	// デストラクタ
	virtual ~SceneBase();

	// 初期化
	virtual void Initialize() = 0;
	// 更新
	//virtual void Update(DX::StepTimer const& timer) = 0;
	virtual void Update(DX::StepTimer const& timer, Game* game) = 0;
	// 描画
	//virtual void Render() = 0;
	virtual void Render(Game* game) = 0;
	// 終了
	virtual void Finalize() = 0;
};