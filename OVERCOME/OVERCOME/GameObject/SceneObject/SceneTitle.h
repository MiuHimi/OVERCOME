//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/08/15
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
	enum GHOST_DIR
	{
		NONE,
		RIGHT_DIR = 90,
		LEFT_DIR = -90
	};

	bool                                               m_toStageSelectMoveOnChecker;
																				  // ステージセレクトシーンに進めるかどうかのチェック

	float											   m_colorAlpha;			  // α値を変更

	static const int								   MAX_TITLE_STR = 8;		  // タイトル文字列
	std::unique_ptr<Obj2D>							   mp_title[MAX_TITLE_STR];	  // タイトルオブジェクト
	static const int								   TITLE_STR_WIDTH;			  // タイトル文字の幅
	std::unique_ptr<Obj2D>							   mp_startBtn;				  // スタートボタンオブジェクト
	std::unique_ptr<Obj2D>							   mp_fade;					  // フェード画像オブジェクト

	std::unique_ptr<GameCamera>						   mp_camera;			      // ゲームカメラオブジェクト

	static const int								   MAX_GHOST_POS_X;			  // 幽霊がいけるX軸の最大値
	static const float								   MAX_GHOST_POS_Y;			  // 幽霊がいけるY軸の最大値
	static const float								   MIN_GHOST_POS_Y;			  // 幽霊がいけるY軸の最小値
	static const int								   RESPAWN_COUNT_MIN;		  // 幽霊のリスポーンカウント最小値
	static const int								   RESPAWN_COUNT_MAX;		  // 幽霊のリスポーンカウント最大値
	GHOST_DIR										   m_ghostDir;				  // 幽霊の向き
	DirectX::SimpleMath::Vector3					   m_ghostPos;				  // 幽霊の位置
	int												   m_spawnCount;			  // 次スポーンするためのカウント
	int												   m_nextSpawnCount;		  // スポーンするまでのカウント

	std::unique_ptr<DirectX::Model>					   mp_modelHouse;			  // 家モデルオブジェクト
	std::unique_ptr<DirectX::Model>					   mp_modelEnemy;			  // 敵モデルオブジェクト

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
