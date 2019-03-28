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
	DirectX::SimpleMath::Vector3             m_vel;                   // 速度

	bool                                     m_state;                 // 生存
	Collision::Sphere                        m_sphere;                // 衝突判定情報

	DirectX::SimpleMath::Matrix              m_rotaY;                 // 回転
	DirectX::SimpleMath::Matrix              m_world;                 // ワールド座標

	std::unique_ptr<DirectX::Model>          mp_modelEnemy;          // 弾モデルオブジェクト

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

	// 位置設定
	void SetPos(DirectX::SimpleMath::Vector3 pos) { m_pos = pos; }
	// 速度ベクトル設定
	void SetVel(DirectX::SimpleMath::Vector3 vel) { m_vel = vel; }

	// 座標取得
	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }

	// 生存状態設定、取得
	bool GetState()               { return m_state; }
	void SetState(bool stateFlag) { m_state = stateFlag; }

	// 衝突判定状態設定、取得
	Collision::Sphere GetCollide() { return m_sphere; }
	void SetCollide(Collision::Sphere collide) { m_sphere = collide; }
	// 回転角を設定
	void SetRotate(float angle) { m_rotaY = DirectX::SimpleMath::Matrix::CreateRotationY(DirectX::XMConvertToRadians(angle)); }

	// モデル情報取得
	const DirectX::Model& GetModel() const
	{
		if (!mp_modelEnemy)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_modelEnemy;
	}
	// モデル情報設定
	void SetModel(std::unique_ptr<DirectX::Model>&& newData)
	{
		mp_modelEnemy = std::move(newData);
	}


private:
	// フォグの設定
	void SetFogEffectDistance(float start, float end);

};