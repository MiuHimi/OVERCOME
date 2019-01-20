//////////////////////////////////////////////////////////////
// File.    Player.h
// Summary. PlayerClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

#include "GameBulletManager.h"
#include "GameCamera.h"

class GameCamera;
class GameRoad;
class MatrixManager;
class Player : public CollisionBox
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                       // 位置
	DirectX::SimpleMath::Vector3             m_vel;                       // 速度(ベクトル)
	float                                    m_direction;                 // 向き(角度)
	DirectX::SimpleMath::Quaternion          m_rotation;                  // 回転
	float                                    m_height;                    // プレイヤー自身の高さ
	float                                    m_jumpForce;                 // ジャンプ力
	float                                    m_gravity;                   // 重力
	float                                    m_fallingPower;              // そのまま落ちるときの力

	bool                                     m_isJump           = false;  // ジャンプしたかを判定
	bool                                     m_collideToFloor   = false;  // 床と接触したかを判定
	bool                                     m_collideToRoad    = false;  // 道路と接触したかを判定
	bool                                     m_noTouchObectFlag = false;  // 何にも触れずジャンプもしていない時にフラグが立つ

	bool                                     m_restartFlag;
	int                                      m_restartTime;

	bool   m_playStartFlag;
	int    m_playStartTime;

	DirectX::SimpleMath::Vector3             m_ahead;

	DirectX::SimpleMath::Matrix              m_world;                     // ワールド座標

	std::unique_ptr<DirectX::Model>          m_modelPlayer;               // プレイヤーモデルオブジェクト
	//std::unique_ptr<GameBulletManager>       mp_bulletManager;
	GameBulletManager*                       mp_bulletManager;
	std::unique_ptr<GameCamera>              mp_gameCamera;               // カメラポインター
	std::unique_ptr<GameRoad>                mp_gameRoad;                 // 道路ポインタ
	DirectX::SimpleMath::Vector2             m_passedRoadPos;             // 通過済みの道路を記憶
	DirectX::SimpleMath::Vector2             m_nextPos;                   // 次に向かう道路の座標を記憶

	bool m_velFlag;

	DirectX::SimpleMath::Vector2                         m_posRestart;               // リスタート位置
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureRestart;           // テクスチャハンドル(リスタート)

	const int                                            m_countUISize = 80;         // 数字のサイズ
	DirectX::SimpleMath::Vector2                         m_posCount;                 // カウント位置
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureCount;             // テクスチャハンドル(カウント)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_texturePointer;           // テクスチャハンドル(ポインター)

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	Player();
	// デストラクタ
	~Player();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render(MatrixManager* matrixManager);
	// 廃棄処理
	void Depose();

	// プレイヤーの位置を取得
	DirectX::SimpleMath::Vector3 GetPos()           { return m_pos; }
	// プレイヤーの向きを取得
	float GetDirection()                            { return m_direction; }
	// プレイヤーの高さを取得
	float GetHeight()                               { return m_height; }
	// プレイヤーがジャンプ中であるかを取得
	bool GetJumpState()                             { return m_isJump; }
	// プレイヤー情報の取得
	Player* GetPlayer();

	// プレイヤーの高さのみの位置を設定
	void SetHeightPos(float pos)                    { m_pos.y = pos; }
	// 道路からそのまま落ちていく速度を設定
	void SetFallingPower(float power)               { m_fallingPower = power; }

	// ジャンプモーションを終了させる(ためのフラグ設定)
	void SetJumpState(bool flag)                    { m_isJump = flag; }
	// 床、道路との衝突判定のフラグ設定
	void SetFloorCollideState(bool flag)            { m_collideToFloor = flag; }
	void SetRoadCollideState(bool flag)             { m_collideToRoad = flag; }
	// 何にも触れずジャンプもしていない時にフラグが立つ
	void SetNotTouchState(bool flag)                { m_noTouchObectFlag = flag; }

	// 進行ベクトルの取得
	DirectX::SimpleMath::Vector3 GetAhead() { return m_ahead; }

	/*// 弾情報取得
	const GameBulletManager& GetBulletManager() const
	{
		if (!mp_bulletManager)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_bulletManager;
	}
	// 弾情報設定
	void SetBulletManager(std::unique_ptr<GameBulletManager>&& newData)
	{
		mp_bulletManager = std::move(newData);
	}*/

	GameBulletManager* GetBulletManager()
	{
		return mp_bulletManager;
	}

private:


};