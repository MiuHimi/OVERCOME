/*
	CollisionSphere.cpp
	境界球を持ったObj3Dクラス
*/

// ヘッダをインクルード
#include "../pch.h"
#include "../Game.h"
#include "CollisionSphere.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

/// <summary>
/// 衝突判定情報の設定
/// </summary>
/// <param name="sphere">衝突判定情報</param>
void CollisionSphere::SetCollision(Collision::Sphere sphere)
{
	// 衝突判定情報を設定
	m_collision = sphere;

	// デバッグ用モデルの作成
	m_dbgObj = std::make_unique<DebugSphere>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), m_collision.c, m_collision.r);

}

/// <summary>
/// 衝突判定情報の取得
/// </summary>
/// <returns>衝突判定球情報</returns>
Collision::Sphere CollisionSphere::GetCollision()
{   // 「ローカルな座標をワールド座標系に変換」
	Collision::Sphere sphere;

	// 境界球の中心座標をワールド行列に座標変換する
	sphere.c = DirectX::SimpleMath::Vector3::Transform(m_collision.c, m_world);
	sphere.r = m_collision.r;

	return sphere;
}

/// <summary>
/// デバッグ用オブジェクト表示関数
/// </summary>
void CollisionSphere::DrawDebugCollision()
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// デバッグ用オブジェクトの表示
	m_dbgObj->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		           world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());

}
