//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2019/09/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../ExclusiveGameObject/CollisionBox.h"

#include "GameRoad.h"
#include "GameBulletManager.h"
#include "GameEnemyManager.h"

#include "../2DObject/Obj2D.h"

class GameEnemyManager;
class GameRoad;
class MatrixManager;
class Player : public CollisionBox
{
// メンバー変数
public:

	// プレイヤーの状態
	enum PLAYER_STATE
	{
		MORIBUND = 3,	// 瀕死
		WOUND = 7,		// 負傷
		NORMAL = 10		// 通常
	};

private:
	DirectX::SimpleMath::Vector3             m_pos;                       // 位置
	DirectX::SimpleMath::Vector3             m_vel;                       // 速度(ベクトル)
	DirectX::SimpleMath::Vector3             m_dir;                       // 移動方向(ベクトル)
	float                                    m_height;                    // プレイヤー自身の高さ
	DirectX::SimpleMath::Vector3             m_posTmp;                    // 前フレームの位置
	int										 m_hp;						  // 体力
	DirectX::SimpleMath::Vector2			 m_hpBasePos;				  // 体力バー基準位置
	int										 m_damageCount;				  // ダメージを受けた時の計測カウント
	static const int                         DAMAGE_EFFECT_COUNT;		  // ダメージ演出の切り替えフレーム数
	static const int                         FINISH_DAMAGE_EFFECT_COUNT;  // 動き出すまでにかかる時間(フレーム数)
	bool									 m_isDamaged;				  // 敵に接触したか

	bool                                     m_playStartFlag;             // プレイヤーが進行し出したらフラグが立つ(初回)
	int                                      m_moveStartCountDown;        // 動き出すまでの時間
	static const int                         START_COUNT_TIME;			  // 動き出すまでにかかる時間(フレーム数)

	bool                                     m_spawnFlag;                 // 敵が出てくるフラグ
	float                                    m_spawnElapsedTime;          // 敵が出現してからの経過時間
	static const int                         SPAWNTIME;                   // 敵が出てくる時間(フレーム数)

	DirectX::SimpleMath::Vector2             m_passingRoadPos;            // 通過済みの道路(ID)を記憶
	DirectX::SimpleMath::Vector2             m_passedRoadPos;             // 通過済みの道路を記憶
	DirectX::SimpleMath::Vector2             m_nextPos;                   // 次に向かう道路の座標を記憶
	bool                                     m_velFlag;                   // 次の道路マスに向けて移動中ならフラグが立つ

	DirectX::SimpleMath::Matrix              m_world;                     // ワールド座標

	//std::unique_ptr<DirectX::Model>          m_modelPlayer;               // プレイヤーモデルオブジェクト
	GameBulletManager*                       mp_bulletManager;            // 弾管理ポインター
	std::unique_ptr<GameRoad>                mp_gameRoad;                 // 道路ポインタ
	
	bool									 m_isFullScreen;			  // フルスクリーンかどうか

	static const int                         COUNT_UI_SIZE;               // カウントダウン数字のサイズ
	std::unique_ptr<Obj2D>					 mp_startCount;				  // スタートカウントダウンオブジェクト
	std::unique_ptr<Obj2D>					 mp_shootPointer;			  // 発射ポインターオブジェクト
	std::unique_ptr<Obj2D>					 mp_dengerousSign;			  // 危険サインオブジェクト
	std::unique_ptr<Obj2D>					 mp_hp;						  // HPオブジェクト
	std::unique_ptr<Obj2D>					 mp_hpFrame;				  // HPのフレームオブジェクト
	std::unique_ptr<Obj2D>					 mp_damageEffect;			  // ダメージ効果オブジェクト

// メンバー関数
public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 生成
	void Create(const bool isFulleScreen);
	// 更新
	bool Update(DX::StepTimer const& timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir, DirectX::SimpleMath::Vector3 correctPos);
	// 描画
	void Render(MatrixManager* matrixManager, GameEnemyManager::DANGERDIRECTION dangerDir);
	// 廃棄
	void Depose();

	//-----------------------------------Getter-----------------------------------//

	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 GetPos()           { return m_pos; }
	// プレイヤーの移動方向を取得
	DirectX::SimpleMath::Vector3 GetDir()           { return m_dir; }
	// プレイヤーの高さを取得
	float GetHeight()                               { return m_height; }
	// HPを取得
	int GetHP()										{ return m_hp; }
	// プレイヤーが動き出したかどうか
	bool GetPlaying()								{ return m_playStartFlag; }
	// 敵が出てくる時間かどうか
	bool GetEnemyTime()								{return m_spawnFlag;}
	// 通過中の道路(ID)
	DirectX::SimpleMath::Vector2 GetPassingRoad()   { return m_passingRoadPos; }
	// プレイヤー情報の取得
	GameBulletManager* GetBulletManager()           { return mp_bulletManager; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	// プレイヤーの高さのみの位置を設定
	void SetHeightPos(float pos)                    { m_pos.y = pos; }
	// 敵に接触したかどうかを設定
	void SetDamaged(bool flag)						{ m_isDamaged = flag; }
	//----------------------------------------------------------------------------//

	// 体力設定
	void Damage(int damage) { m_hp -= damage; }

private:

	// 進みだす前の処理
	bool BeforeGoOnPlayer(const bool isClickCenter);

	// 進み始めた後の処理
	void AfterGoOnPlayer(const bool isGoOn, DX::StepTimer const& timer, DirectX::SimpleMath::Vector3& cameraDir);

	// 次の進路先を調べる
	void SearchNextRoad(const DirectX::SimpleMath::Vector2 nowPos);

	// ダメージの更新
	void UpdateDamage();

	// HPの更新
	void UpdateHP();

};