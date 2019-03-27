//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"

#include "../../Game.h"

#include "../3DObject/Player.h"
#include "../3DObject/GameCamera.h"

#include "../3DObject/GameFloor.h"
#include "../3DObject/GameRoad.h"
#include "../3DObject/GameTarget.h"

#include "../3DObject/GameMap.h"
#include "../3DObject/GameEnemyManager.h"

//#include "../SkyDome.h"

#include "../2DObject/GameTimer.h"
#include "../2DObject/GameScore.h"

#include "../ExclusiveGameObject/CollisionMesh.h"
#include "../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class ScenePlay : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toResultMoveOnChecker;                 // リザルトシーンに進めるかどうかのチェック
	bool m_returnToTitleChecker;                  // タイトルシーンに戻れるかどうかのチェック

	std::unique_ptr<GameCamera> mp_camera;        // カメラオブジェクト

	std::unique_ptr<GameFloor> mp_gameFloor;	  // ゲーム床オブジェクト
	std::unique_ptr<GameRoad>  mp_gameRoad;	      // ゲーム道路オブジェクト
	std::unique_ptr<GameTarget> mp_gameTarget;    // ゲーム標的オブジェクト

	std::unique_ptr<GameMap>   mp_gameMap;    // ゲームマップオブジェクト
	std::unique_ptr<GameEnemyManager>   mp_gameEnemyManager;    // ゲーム敵管理オブジェクト

	static std::unique_ptr<Player> mp_player;     // プレイヤーオブジェクト

	//std::unique_ptr<SkyDome> mp_skydome;          // スカイドームオブジェクト

	std::unique_ptr<GameTimer> mp_gameTimer;      // 制限時間オブジェクト
	std::unique_ptr<GameScore> mp_gameScore;      // スコアオブジェクト

	//bool m_hitPlayerToFloorFlag = false;
	//bool m_hitPlayerToRoadFlag = false;

	float m_fadeInCount;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;        // テクスチャハンドル(背景)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureFadeIn;            // テクスチャハンドル(フェードイン)

	MatrixManager*                                       mp_matrixManager;      // 行列管理変数

	std::unique_ptr<CollisionMesh>                       m_box;
	EffectManager*	                                     mp_effectManager;      // エフェクト管理変数

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	ScenePlay(SceneManager* sceneManager);
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

	static Player* GetPlayer() { return mp_player.get(); }

};
