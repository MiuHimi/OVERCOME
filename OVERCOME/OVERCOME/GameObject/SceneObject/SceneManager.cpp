//////////////////////////////////////////////////////////////
// File.    SceneManager.cpp
// Summary. SceneManagerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneBase.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "ScenePlay.h"
#include "SceneResult.h"

// usingディレクトリ



/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="startSceneId">開始シーンのID</param>
SceneManager::SceneManager(SceneId startSceneId)
	                     : mp_activeScene(nullptr)
	                     , mp_requestedScene(nullptr)
{
	// シーンリストの初期化
	for (int i = 0; i < NUM_SCENES; i++)
	{
		mp_scenes[i] = nullptr;
	}

	// シーンオブジェクトをリストに登録
	mp_scenes[SCENE_LOGO] = new SceneLogo(mp_game, this);
	mp_scenes[SCENE_TITLE] = new SceneTitle(mp_game, this);
	mp_scenes[SCENE_PLAY] = new ScenePlay(mp_game, this);

	// 開始シーンの設定
	RequestToChangeScene(startSceneId);
}
/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">Gameファイルの情報</param>
/// <param name="startSceneId">開始シーンのID</param>
SceneManager::SceneManager(Game* game, SceneId startSceneId)
	: mp_activeScene(nullptr)
	, mp_requestedScene(nullptr)
{
	// シーンリストの初期化
	for (int i = 0; i < NUM_SCENES; i++)
	{
		mp_scenes[i] = nullptr;
	}

	// シーンオブジェクトをリストに登録
	mp_scenes[SCENE_LOGO] = new SceneLogo(game, this);
	mp_scenes[SCENE_TITLE] = new SceneTitle(game, this);
	mp_scenes[SCENE_PLAY] = new ScenePlay(game, this);
	mp_scenes[SCENE_RESULT] = new SceneResult(game, this);

	// 開始シーンの設定
	RequestToChangeScene(startSceneId);
}
/// <summary>
/// デストラクタ
/// </summary>
SceneManager::~SceneManager()
{
	mp_activeScene = nullptr;
	mp_requestedScene = nullptr;

	for (int i = 0; i < NUM_SCENES; i++)
	{
		delete mp_scenes[i];
		mp_scenes[i] = nullptr;
	}
}

/// <summary>
/// 更新中のシーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneManager::UpdateActiveScene(DX::StepTimer const& timer, Game* game)
{
	// シーンの要求があったら
	if (mp_requestedScene != nullptr)
	{
		// シーンの変更
		ChangeScene();
	}

	// 更新中のシーンがあったら
	if (mp_activeScene != nullptr)
	{
		// シーンの更新
		//mp_activeScene->Update(timer);
		mp_activeScene->Update(timer, game);
	}
}

/// <summary>
/// 活動中のシーンの描画処理
/// </summary>
void SceneManager::RenderActiveScene(DirectX::SpriteBatch* sprites, Game* game)
{
	// 更新中のシーンがあったら
	if (mp_activeScene != nullptr)
	{
		// シーンの描画
		mp_activeScene->Render(sprites, game);
	}
}

/// <summary>
/// シーンの変更を要求
/// </summary>
/// <param name="sceneId">変更を要求するシーンのID</param>
/// <returns>要求が通った場合 true, 通らない場合 false</returns>
bool SceneManager::RequestToChangeScene(SceneId sceneId)
{
	if (sceneId >= 0 && sceneId < NUM_SCENES)
	{
		mp_requestedScene = mp_scenes[sceneId];
		return true;
	}
	else
	{
		mp_requestedScene = nullptr;
		return false;
	}
}

/// <summary>
/// 活動シーンの変更
/// </summary>
void SceneManager::ChangeScene()
{
	// 活動中のシーンを終了させる
	if (mp_activeScene != nullptr)
	{
		mp_activeScene->Finalize();
	}

	// 要求されたシーンを活動中にする
	mp_activeScene = mp_requestedScene;
	mp_requestedScene = nullptr;

	// 新たに活動中になったシーンを初期化する
	if (mp_activeScene != nullptr)
	{
		mp_activeScene->Initialize();
	}
}