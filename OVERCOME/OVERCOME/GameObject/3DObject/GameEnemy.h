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

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // 位置
	DirectX::SimpleMath::Vector3             m_posTmp;                // 前フレームの位置
	DirectX::SimpleMath::Vector3             m_vel;                   // 速度
	DirectX::SimpleMath::Vector3             m_dir;                   // 向き

	bool                                     m_state;                 // 生存
	bool									 m_shock;                 // 自機の弾が当たったタイミングのみフラグが立つ
	Collision::Sphere                        m_sphere;                // 衝突判定情報

	DirectX::SimpleMath::Quaternion          m_rotaX;                 // X軸回転
	DirectX::SimpleMath::Quaternion          m_rotaY;                 // Y軸回転
	DirectX::SimpleMath::Matrix              m_world;                 // ワールド座標

	std::unique_ptr<DirectX::Model>          mp_modelEnemy;           // 弾モデルオブジェクト

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameEnemy(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model);
	// デストラクタ
	~GameEnemy();

	// 生成
	void Create();

	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render(MatrixManager* matrixManager);


	//-----------------------------------Getter-----------------------------------//

	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }
	DirectX::SimpleMath::Vector3 GetVel() { return m_vel; }
	DirectX::SimpleMath::Vector3 GetDir() { return m_dir; }
	DirectX::SimpleMath::Matrix GetMatrix() { return m_world; }

	bool GetState()						  { return m_state; }
	bool GetShock()						  { return m_shock; }
	Collision::Sphere GetCollide()		  { return m_sphere; }

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
	void SetCollide(Collision::Sphere collide)				 { m_sphere = collide; }

	void SetRotateX(float angle)							 { m_rotaX = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(1.0f, 0.0f, 0.0f), angle); }
	void SetRotateY(float angle)							 { m_rotaY = DirectX::SimpleMath::Quaternion::CreateFromAxisAngle(DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f), angle); }

	void SetModel(std::unique_ptr<DirectX::Model>&& newData) { mp_modelEnemy = std::move(newData); }
	//----------------------------------------------------------------------------//

private:
	// フォグの設定
	void SetFogEffectDistance(float start, float end);

};