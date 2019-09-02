//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2018/07/27
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
//public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                       // 位置
	DirectX::SimpleMath::Vector3             m_vel;                       // 速度(ベクトル)
	DirectX::SimpleMath::Vector3             m_dir;                       // 移動方向(ベクトル)
	float                                    m_height;                    // プレイヤー自身の高さ
	float                                    m_jumpForce;                 // ジャンプ力
	float                                    m_gravity;                   // 重力
	DirectX::SimpleMath::Vector3             m_posTmp;                    // 前フレームの位置

	bool                                     m_playStartFlag;             // ゲームが開始したらフラグが立つ
	int                                      m_playStartTime;             // ゲームが開始されるまでの時間

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
	
	std::unique_ptr<Obj2D>					 mp_startGuide;				  // スタート案内オブジェクト

	const int                                COUNTUISIZE = 80;            // 数字のサイズ
	DirectX::SimpleMath::Vector2             m_posCountUI;                // カウント位置
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		                                     m_textureCount;              // テクスチャハンドル(カウント)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     
		                                     m_texturePointer;            // テクスチャハンドル(ポインター)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
											 m_textureDengerous;          // テクスチャハンドル(危険サイン)

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
	bool Update(DX::StepTimer const& timer, const bool isPlayFlag, DirectX::SimpleMath::Vector3& cameraDir);
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
	// プレイヤーが動き出したかどうか
	bool GetPlaying()								{ return m_playStartFlag; }
	// 敵が出てくる時間かどうか
	bool GetEnemyTime()								{return m_spawnFlag;}
	// 通過中の道路(ID)
	DirectX::SimpleMath::Vector2 GetPassingRoad()   { return m_passingRoadPos; }
	// プレイヤー情報の取得
	Player* GetPlayer();
	// プレイヤー情報の取得
	GameBulletManager* GetBulletManager()           { return mp_bulletManager; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	// プレイヤーの高さのみの位置を設定
	void SetHeightPos(float pos)                    { m_pos.y = pos; }
	//----------------------------------------------------------------------------//

//private:

};