//////////////////////////////////////////////////////////////
// File.    ScenePlay.h
// Summary. ScenePlayClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../../Game.h"

#include "../MyCamera.h"

#include "../Player.h"

#include "../GameFloor.h"
#include "../GameRoad.h"

#include "../SkyDome.h"

#include "../GameTimer.h"
#include "../GameScore.h"

class ScenePlay : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toResultMoveOnChecker;                 // リザルトシーンに進めるかどうかのチェック
	bool m_returnToTitleChecker;                  // タイトルシーンに戻れるかどうかのチェック

	Game* mp_game;                                // Gameファイルの情報を格納
	std::unique_ptr<DirectX::SpriteFont> m_font;  // スプライトフォント

	std::unique_ptr<MyCamera> mp_camera;          // カメラオブジェクト

	std::unique_ptr<GameFloor> mp_gameFloor;	  // ゲーム床オブジェクト
	std::unique_ptr<GameRoad>  mp_gameRoad;	      // ゲーム道路オブジェクト

	static std::unique_ptr<Player> mp_player;     // プレイヤーオブジェクト

	std::unique_ptr<SkyDome> mp_skydome;          // スカイドームオブジェクト

	std::unique_ptr<GameTimer> mp_gameTimer;      // 制限時間オブジェクト
	std::unique_ptr<GameScore> mp_gameScore;      // スコアオブジェクト

	//bool m_hitPlayerToFloorFlag = false;
	//bool m_hitPlayerToRoadFlag = false;

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	ScenePlay(SceneManager* sceneManager);
	ScenePlay(Game* game, SceneManager* sceneManager);
	// デストラクタ
	~ScenePlay();

	// 初期化
	virtual void Initialize() override;
	// 更新
	//virtual void Update(DX::StepTimer const& timer) override;
	virtual void Update(DX::StepTimer const& timer, Game* game) override;
	// 描画
	//virtual void Render() override;
	virtual void Render(DirectX::SpriteBatch* sprites, Game* game) override;
	// 終了
	virtual void Finalize() override;

	static Player* GetPlayer() { return mp_player.get(); }

};
