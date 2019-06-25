//////////////////////////////////////////////////////////////
// File.    SceneManager.h
// Summary. SceneManagerClass
// Date.    2018/11/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

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

class SceneBase;
// シーン管理クラス
class SceneManager
{
// メンバー変数(構造体、enum、列挙子 etc...)
public :
	//static int            m_stageID;                   // ステージ番号
	static bool           m_clearSceneState;           // リザルトシーンの状態

	static const int      MAXSTAGE;                    // 全ステージ数

private:
	// 他クラスへのポインタ
	SceneBase*            mp_scene;                    // シーンが設定される

	static SceneId        m_activeScene;               // アクティブなシーンを保存
	SceneId               m_nextScene;                 // 次に更新するシーンを決定
	bool                  m_requestSceneFlag;          // シーン遷移を要求されたらフラグが立つ

	bool                  m_gameStateFlag;             // ゲームの更新中はフラグが立つ

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneManager(SceneId startSceneId);
	// デストラクタ
	~SceneManager();

	// 更新中のシーンの更新処理
	void UpdateActiveScene(DX::StepTimer const& timer);
	// 更新中のシーンの描画処理
	void RenderActiveScene();
	// 遷移したいシーンを要求
	bool RequestToChangeScene(SceneId sceneId);
	// シーンを削除
	void DeleteScene();

	//-----------------------------------Getter-----------------------------------//

	// ステージ番号の取得
	//static int  GetStageNum()					{ return m_stageID; }

	// リザルトシーンの状態の取得
	static bool GetResultSceneState()			{ return m_clearSceneState; }

	// アクティブなシーンの取得
	static SceneId GetActiveScene()				{ return m_activeScene; }

	// ゲームの更新中フラグの取得
	bool GetGameState()							{ return m_gameStateFlag; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	// ステージ番号の設定、取得
	//static void SetStageNum(int stageID)		{ m_stageID = stageID; }

	// リザルトシーンの状態の設定、取得
	static void SetResultSceneState(bool state) { m_clearSceneState = state; }

	// ゲームの更新中フラグの設定
	void SetGameState(bool flag)				{ m_gameStateFlag = flag; }
	//----------------------------------------------------------------------------//

private:
	// シーンを遷移する
	void ChangeScene();

};
