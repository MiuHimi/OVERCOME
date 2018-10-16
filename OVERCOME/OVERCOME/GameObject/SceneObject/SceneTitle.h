//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteFont.h>
#include "SceneBase.h"

class SceneTitle : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toPlayMoveOnChecker;   // ロゴシーンに進めるかどうかのチェック

	Game* mp_game;                                // Gameファイルの情報を格納

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneTitle(SceneManager* sceneManager);
	// デストラクタ
	~SceneTitle();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 描画
	virtual void Finalize() override;
};
