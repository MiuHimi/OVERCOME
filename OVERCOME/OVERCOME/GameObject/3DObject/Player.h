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
	DirectX::SimpleMath::Vector3             m_dir;                       // 移動方向(ベクトル)
	float                                    m_height;                    // プレイヤー自身の高さ
	float                                    m_jumpForce;                 // ジャンプ力
	float                                    m_gravity;                   // 重力
	DirectX::SimpleMath::Vector3             m_posTmp;                    // 前フレームの位置

	bool                                     m_playStartFlag;             // ゲームが開始したらフラグが立つ
	int                                      m_playStartTime;             // ゲームが開始されるまでの時間
	bool                                     m_restartFlag;               // リスタートする条件が揃ったらフラグが立つ
	int                                      m_restartTime;               // リスタートするまでの時間

	bool flag;
	int count;

	DirectX::SimpleMath::Vector2             m_passingRoadPos;            // 通過済みの道路(ID)を記憶
	DirectX::SimpleMath::Vector2             m_passedRoadPos;             // 通過済みの道路を記憶
	DirectX::SimpleMath::Vector2             m_nextPos;                   // 次に向かう道路の座標を記憶
	bool                                     m_velFlag;                   // 次の道路マスに向けて移動中ならフラグが立つ

	DirectX::SimpleMath::Matrix              m_world;                     // ワールド座標

	//std::unique_ptr<DirectX::Model>          m_modelPlayer;               // プレイヤーモデルオブジェクト
	GameBulletManager*                       mp_bulletManager;            // 弾管理ポインター
	std::unique_ptr<GameCamera>              mp_gameCamera;               // カメラポインター
	std::unique_ptr<GameRoad>                mp_gameRoad;                 // 道路ポインタ
	
	DirectX::SimpleMath::Vector2             m_posRestartUI;              // リスタートUI位置
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     
		                                     m_textureRestart;            // テクスチャハンドル(リスタート)

	const int                                COUNTUISIZE = 80;            // 数字のサイズ
	DirectX::SimpleMath::Vector2             m_posCountUI;                // カウント位置
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>
		                                     m_textureCount;              // テクスチャハンドル(カウント)

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     
		                                     m_texturePointer;            // テクスチャハンドル(ポインター)

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

private:


};