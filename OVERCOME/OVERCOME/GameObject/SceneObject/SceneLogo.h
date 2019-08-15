//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2019/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"

class MatrixManager;
class SceneLogo : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
//public:

private:
	bool                                               m_toTitleMoveOnChecker;		// タイトルシーンに進めるかどうかのチェック

	static const int                                   FADEOUT_NEED_SECOND;			// フェードアウトし始める秒数

	std::unique_ptr<Obj2D>							   mp_background;				// 背景オブジェクト
	std::unique_ptr<Obj2D>							   mp_logo;						// ロゴオブジェクト
	std::unique_ptr<Obj2D>							   mp_fade;						// フェード画像オブジェクト

	MatrixManager*                                     mp_matrixManager;			// 行列管理変数

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneLogo(SceneManager* sceneManager, bool isFullScreen);
	// デストラクタ
	~SceneLogo();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 終了
	virtual void Finalize() override;

//private:

};
