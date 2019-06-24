//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2019/06/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"

#include "../../Game.h"

#include "../3DObject/Player.h"
#include "../3DObject/GameCamera.h"
#include "../3DObject/GameEnemyManager.h"

#include "../3DObject/GameRoad.h"
#include "../3DObject/GameTarget.h"

#include "../3DObject/GameMap.h"

#include "../2DObject/GameScore.h"

//#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class ScenePlay : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool								m_toResultMoveOnChecker;   // リザルトシーンに進めるかどうかのチェック
	bool								m_returnToTitleChecker;    // タイトルシーンに戻れるかどうかのチェック

	static std::unique_ptr<Player>		mp_player;				   // プレイヤーオブジェクト

	std::unique_ptr<GameCamera>			mp_camera;				   // カメラオブジェクト
	std::unique_ptr<GameEnemyManager>   mp_gameEnemyManager;	   // ゲーム敵管理オブジェクト

	std::unique_ptr<GameRoad>			mp_gameRoad;			   // ゲーム道路オブジェクト
	std::unique_ptr<GameTarget>			mp_gameTarget;			   // ゲーム標的オブジェクト
	std::unique_ptr<GameMap>			mp_gameMap;				   // ゲームマップオブジェクト

	std::unique_ptr<GameScore> mp_gameScore;					   // スコアオブジェクト

	float m_fadeInCount;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
										m_textureBackground;	   // テクスチャハンドル(背景)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
										m_textureFadeIn;		   // テクスチャハンドル(フェードイン)

	MatrixManager*						mp_matrixManager;		   // 行列管理変数

	//EffectManager*					mp_effectManager;		   // エフェクト管理変数

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
