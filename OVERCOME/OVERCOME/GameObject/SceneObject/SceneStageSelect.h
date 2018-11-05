//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteFont.h>
#include "SceneBase.h"

class SceneStageSelect : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toPlayMoveOnChecker;                   // プレイシーンに進めるかどうかのチェック
	bool m_returnToTitleChecker;                  // タイトルシーンに戻れるかどうかのチェック

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneStageSelect(SceneManager* sceneManager);
	// デストラクタ
	~SceneStageSelect();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 終了
	virtual void Finalize() override;

};
