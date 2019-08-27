//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2019/08/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"

class MatrixManager;
class SceneStageSelect : public SceneBase
{
// メンバー変数
//public:

private:
	bool                                               m_toPlayMoveOnChecker;     // タイトルシーンに進めるかどうかのチェック

	int												   m_selectedStage;			  // 選択されたステージ

	// ステージ数
	enum STAGE
	{
		ONE,
		TWO,

		NUM
	};

	static const int                                   STAGE_ICON_SIZE;			  // ステージアイコンサイズ

	std::unique_ptr<Obj2D>							   mp_background;			  // 背景オブジェクト
	std::unique_ptr<Obj2D>							   mp_stageSelectImage;		  // StageSelectオブジェクト
	std::unique_ptr<Obj2D>							   mp_stageNum[NUM];		  // ステージ番号オブジェクト
	std::unique_ptr<Obj2D>							   mp_stageFlame[NUM];		  // ステージ番号フレームオブジェクト
	std::unique_ptr<Obj2D>							   mp_fade;					  // フェード画像オブジェクト

	MatrixManager*                                     mp_matrixManager;		  // 行列管理変数

// メンバー関数
public:
	// コンストラクタ
	SceneStageSelect(SceneManager* sceneManager, bool isFullScreen);
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

//private:

};
