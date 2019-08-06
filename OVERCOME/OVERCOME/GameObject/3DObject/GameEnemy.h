//////////////////////////////////////////////////////////////
// File.    GameEnemy.h
// Summary. GameEnemyClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class MatrixManager;
class GameEnemy : public CollisionSphere
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:
	// 敵の種類
	enum EnemyType
	{
		NORMAL,
		POWER,
		SPEED,

		NUM
	};

	static const int						 NORMAL_ENEMY_POINT = 20; // 通常敵の得点
	static const int						 POWER_ENEMY_POINT = 30;  // パワー敵の得点
	static const int						 SPEED_ENEMY_POINT = 10;  // スピード敵の得点

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // 位置
	DirectX::SimpleMath::Vector3             m_posTmp;                // 前フレームの位置
	DirectX::SimpleMath::Vector3             m_vel;                   // 速度
	DirectX::SimpleMath::Vector3             m_dir;                   // 向き
	float									 m_size;				  // サイズ
	int										 m_prob[EnemyType::NUM];  // 出現確率
	EnemyType								 m_type;				  // 種類
	int										 m_hp;					  // 体力(HP)
	static const int						 NORMAL_ENEMY_PROBABILITY;// 通常敵の生成確率
	static const int						 POWER_ENEMY_PROBABILITY; // パワー敵の生成確率
	static const int						 SPEED_ENEMY_PROBABILITY; // スピード敵の生成確率

	bool                                     m_state;                 // 生存
	bool									 m_shock;                 // 自機の弾が当たったタイミングのみフラグが立つ
	Collision::Sphere                        m_sphere;                // 衝突判定情報

	DirectX::SimpleMath::Quaternion          m_rotaX;                 // X軸回転
	DirectX::SimpleMath::Quaternion          m_rotaY;                 // Y軸回転
	DirectX::SimpleMath::Matrix              m_world;                 // ワールド座標

	std::unique_ptr<DirectX::Model>          mp_modelEnemy;           // 敵モデルオブジェクト

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameEnemy(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model);
	// デストラクタ
	~GameEnemy();

	// 生成
	void Create(int probability);

	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render(MatrixManager* matrixManager);


	//-----------------------------------Getter-----------------------------------//

	DirectX::SimpleMath::Vector3 GetPos()	{ return m_pos; }
	DirectX::SimpleMath::Vector3 GetVel()	{ return m_vel; }
	DirectX::SimpleMath::Vector3 GetDir()	{ return m_dir; }
	float GetSize()							{ return m_size; }
	DirectX::SimpleMath::Matrix GetMatrix() { return m_world; }

	bool GetState()						    { return m_state; }
	bool GetShock()						    { return m_shock; }
	EnemyType GetType()					    { return m_type; }
	int GetHP()								{ return m_hp; }
	Collision::Sphere GetCollide()		    { return m_sphere; }

	const DirectX::Model& GetModel() const
	{
		if (!mp_modelEnemy)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_modelEnemy;
	}
	//----------------------------------------------------------------------------//


	//-----------------------------------Setter-----------------------------------//

	void SetPos(DirectX::SimpleMath::Vector3 pos)			 { m_pos = pos; }
	void SetVel(DirectX::SimpleMath::Vector3 vel)			 { m_vel = vel; }
	void SetDir(DirectX::SimpleMath::Vector3 dir)			 { m_dir = dir; }

	void SetState(bool stateFlag)							 { m_state = stateFlag; }
	void SetShock(bool shockFlag)							 { m_shock = shockFlag; }
	void SetHP(int hp)										 { m_hp = hp; }
	void SetCollide(Collision::Sphere collide)				 { m_sphere = collide; }

	void SetRotateX(float angle)							 { m_rotaX = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), angle); }
	void SetRotateY(float angle)							 { m_rotaY = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), angle); }

	void SetModel(std::unique_ptr<DirectX::Model>&& newData) { mp_modelEnemy = std::move(newData); }
	//----------------------------------------------------------------------------//

private:
	// フォグの設定
	void SetFogEffectDistance(float start, float end);

};