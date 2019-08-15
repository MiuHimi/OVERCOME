//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/08/13
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"
#include "../3DObject/GameCamera.h"

#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class SceneTitle : public SceneBase
{
// メンバー変数
//public:

private:
	bool                                               m_toStageSelectMoveOnChecker;
																				  // ステージセレクトシーンに進めるかどうかのチェック

	float											   m_colorAlpha;			  // α値を変更

	std::unique_ptr<Obj2D>							   mp_title;				  // タイトルオブジェクト
	std::unique_ptr<Obj2D>							   mp_startBtn;				  // スタートボタンオブジェクト
	std::unique_ptr<Obj2D>							   mp_fade;					  // フェード画像オブジェクト

	std::unique_ptr<GameCamera>						   mp_camera;			      // ゲームカメラオブジェクト

	std::unique_ptr<DirectX::Model>					   mp_modelHouse;			  // 家モデルオブジェクト

	MatrixManager*                                     mp_matrixManager;          // 行列管理変数

	EffectManager*									   mp_effectManager;		  // エフェクト管理変数

// メンバー関数
public:
	// コンストラクタ
	SceneTitle(SceneManager* sceneManager, bool isFullScreen);
	// デストラクタ
	~SceneTitle();

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
