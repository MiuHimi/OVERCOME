//////////////////////////////////////////////////////////////
// File.    SceneResult.h
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteFont.h>
#include "SceneBase.h"

class SceneResult : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool                                   m_toTitleMoveOnChecker;                 // タイトルシーンに進めるかどうかのチェック
	bool                                   m_returnToPlayChecker;                  // プレイシーンに戻れるかどうかのチェック
	bool                                   m_resultState;                          // リザルトシーンの状態

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneResult(SceneManager* sceneManager);
	// デストラクタ
	~SceneResult();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 終了
	virtual void Finalize() override;

};
