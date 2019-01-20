//////////////////////////////////////////////////////////////
// File.    GameBullet.h
// Summary. GameBulletClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../ExclusiveGameObject/CollisionSphere.h"

class MatrixManager;
class GameBullet : public CollisionSphere
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	DirectX::SimpleMath::Vector3             m_pos;                   // 位置
	DirectX::SimpleMath::Vector3             m_vel;                   // 速度

	bool                                     m_state;                 // 生存
	Collision::Sphere                        m_sphere;                  // 衝突判定情報

	DirectX::SimpleMath::Matrix              m_world;                 // ワールド座標

	std::unique_ptr<DirectX::Model>          mp_modelBullet;          // 弾モデルオブジェクト

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameBullet(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag,DirectX::Model* model);
	// デストラクタ
	~GameBullet();

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

	DirectX::SimpleMath::Vector3 GetVel() { return m_vel; }

	// 座標設定、取得
	DirectX::SimpleMath::Vector3 GetPos() { return m_pos; }
	//void SetPos(DirectX::SimpleMath::Vector3 pos) { m_pos = pos; }

	// 生存状態設定、取得
	bool GetState() { return m_state; }
	void SetState(bool stateFlag) { m_state = stateFlag; }

	// 衝突判定状態設定、取得
	Collision::Sphere GetCollide() { return m_sphere; }
	void SetCollide(Collision::Sphere collide) { m_sphere = collide; }

	// モデル情報設定、取得
	/*DirectX::Model* GetModel() { return mp_modelBullet; }
	void SetModel(DirectX::Model* model) { mp_modelBullet = model; }*/

	// モデル情報取得
	const DirectX::Model& GetModel() const
	{
		if (!mp_modelBullet)
		{
			throw std::domain_error("null pointer");
		}
		return *mp_modelBullet;
	}
	// モデル情報設定
	void SetModel(std::unique_ptr<DirectX::Model>&& newData)
	{
		mp_modelBullet = std::move(newData);
	}


private:
	// フォグの設定
	void SetFogEffectDistance(float start, float end);

};