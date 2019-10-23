//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2019/06/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../../Game.h"

#include "../3DObject/GameCamera.h"

#include "../3DObject/Player.h"

#include "../3DObject/GameEnemy.h"
#include "../3DObject/GameEnemyManager.h"

#include "../3DObject/GameDecorateObject.h"

#include "../3DObject/GameRoad.h"
#include "../3DObject/GameMap.h"

#include "../2DObject/GameScore.h"

#include "../2DObject/Obj2D.h"

class MatrixManager;
class ScenePlay : public SceneBase
{
// メンバー変数
//public:

private:
	bool								m_toResultMoveOnChecker;   // リザルトシーンに進めるかどうかのチェック
	bool								m_returnToTitleChecker;    // タイトルシーンに戻れるかどうかのチェック

	bool								isStartPlay;			   // ゲームをスタートする条件が整ったらフラグが立つ

	std::unique_ptr<GameCamera>			mp_camera;				   // ゲームカメラオブジェクト

	std::unique_ptr<Player>				mp_player;				   // プレイヤーオブジェクト

	std::unique_ptr<GameEnemy>			mp_gameEnemy;			   // 敵オブジェクト
	std::unique_ptr<GameEnemyManager>	mp_gameEnemyManager;	   // 敵管理オブジェクト

	std::unique_ptr<GameRoad>			mp_gameRoad;			   // 道オブジェクト
	std::unique_ptr<GameMap>			mp_gameMap;				   // ステージオブジェクト

	std::unique_ptr<GameDecorateObject>	mp_gameDecorateObject;	   // 装飾品オブジェクト

	std::unique_ptr<GameScore>			mp_gameScore;			   // スコアオブジェクト

	std::unique_ptr<Obj2D>				mp_outline;				   // あらすじオブジェクト
	std::unique_ptr<Obj2D>				mp_fade;				   // フェード画像オブジェクト

	MatrixManager*						mp_matrixManager;		   // 行列管理変数

// メンバー関数
public:
	// コンストラクタ
	ScenePlay(SceneManager* sceneManager, bool isFullScreen);
	// デストラクタ
	~ScenePlay();

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
