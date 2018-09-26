//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class Game;
class Player : public CollisionBox
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // 位置
	DirectX::SimpleMath::Vector3             m_vel;                   // 速度
	float                                    m_direction;             // 向き(角度)
	DirectX::SimpleMath::Quaternion          m_rotation;              // 回転
	float                                    m_height;                // 高さ
	float                                    m_jumpForce;             // ジャンプ力
	float                                    m_gravity;               // 重力

	bool                                     m_isJump = false;        // ジャンプしたかを判定
	bool                                     m_isCollide = false;     // 接触したかを判定

	DirectX::SimpleMath::Matrix              m_world;                 // ワールド座標

	std::unique_ptr<DirectX::Model>          m_modelPlayer;           // プレイヤーモデルオブジェクト
	//std::unique_ptr<CollisionBox>          mp_floor;                // 床衝突判定用オブジェクト

	Game*                                    mp_game;                 // Gameファイルの情報を格納

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	Player();
	Player(Game* game);
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render(DirectX::SimpleMath::Matrix view);
	// 廃棄処理
	void Depose();

	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }
	// プレイヤーの向きを取得
	float GetDirection() { return m_direction; }
	// プレイヤーの高さを取得
	float GetHeight() { return m_height; }
	Player* GetPlayer();

	void SetJumpState(bool flag) { m_isJump = flag; }
	//void SetCollideToRoad(bool flag) { m_isJump = flag; }
	void SetHeightPos(float pos)      { m_pos.y = pos; }
	void SetHeightVel(float vel)      { m_vel.y = vel; }
	void SetCollideState(bool flag)   { m_isCollide = flag; }

private:


};