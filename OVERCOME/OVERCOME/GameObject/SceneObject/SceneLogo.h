//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../../Game.h"

class SceneLogo : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toTitleMoveOnChecker;   // タイトルシーンに進めるかどうかのチェック

	int m_count;                 // タイムカウント変数

	Game* mp_game;                                // Gameファイルの情報を格納

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneLogo(SceneManager* sceneManager);
	SceneLogo(Game* game, SceneManager* sceneManager);
	// デストラクタ
	~SceneLogo();

	// 初期化
	virtual void Initialize() override;
	// 更新
	//virtual void Update(DX::StepTimer const& timer) override;
	virtual void Update(DX::StepTimer const& timer, Game* game) override;
	// 描画
	//virtual void Render() override;
	virtual void Render(Game* game) override;
	// 終了
	virtual void Finalize() override;

};
