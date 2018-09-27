//////////////////////////////////////////////////////////////
// File.    SceneManager.h
// Summary. SceneManagerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../DeviceResources.h"
#include "../../StepTimer.h"

#include "../../Game.h"


// クラスの宣言
class SceneBase;

// シーンID
enum SceneId
{
	SCENE_LOGO,
	SCENE_TITLE,
	SCENE_SELECTSTAGE,
	SCENE_PLAY,
	SCENE_RESULT,

	NUM_SCENES
};


// シーン管理クラス
class Game;
class SceneManager
{
// メンバー変数(構造体、enum、列挙子 etc...)
public :
	static int m_stageNum;

private:
	// 他クラスへのポインタ
	//SceneBase* mp_scenes[NUM_SCENES];    // 登録されているシーンのリスト
	SceneBase* mp_scene;                 // シーンが設定される

	//SceneBase* mp_activeScene;           // 更新中のシーン
	//SceneBase* mp_requestedScene;        // 要求されたシーン

	SceneId m_nextScene;                 // 次に更新するシーンを決定
	bool m_requestSceneFlag;             // シーン遷移を要求されたらフラグが立つ


// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneManager(SceneId startSceneId);
	SceneManager(Game* game, SceneId startSceneId);
	// デストラクタ
	~SceneManager();

	// 更新中のシーンの更新処理
	void UpdateActiveScene(DX::StepTimer const& timer);
	void UpdateActiveScene(DX::StepTimer const& timer, Game* game);
	// 更新中のシーンの描画処理
	void RenderActiveScene(DirectX::SpriteBatch* sprite, Game* game);
	// 遷移したいシーンを要求
	bool RequestToChangeScene(SceneId sceneId);

	// ステージ番号の設定、取得
	static void SetStageNum(int stageID) { m_stageNum = stageID; }
	static int GetStageNum()              { return m_stageNum; }

private:
	// シーンを遷移する
	void ChangeScene(Game* game);

};
