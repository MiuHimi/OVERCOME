/*
	CollisionBox.cpp
	境界箱を持ったObj3Dクラス
*/

// ヘッダをインクルード
#include "../pch.h"
#include "../Game.h"
#include "CollisionBox.h"

/// <summary>
/// 衝突判定情報の設定
/// </summary>
/// <param name="sphere">衝突判定情報</param>
void CollisionBox::SetCollision(Collision::Box box)
{
	// 衝突判定情報を設定
	m_collision = box;

	if (mp_game)
	{
		// デバッグ用モデルの作成
		m_dbgObj = std::make_unique<DebugBox>(mp_game->GetDevice(), m_collision.c, m_collision.r);
	}
}

/// <summary>
/// 衝突判定情報の取得
/// </summary>
/// <returns>衝突判定箱情報</returns>
Collision::Box CollisionBox::GetCollision()
{// 「ローカルな座標をワールド座標系に変換」
	Collision::Box box;

	// 境界箱の中心座標をワールド行列に座標変換する
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	box.c = DirectX::SimpleMath::Vector3::Transform(m_collision.c, world);
	box.r = m_collision.r;

	return box;
}

/// <summary>
/// デバッグ用オブジェクト表示関数
/// </summary>
void CollisionBox::DrawDebugCollision(DirectX::SimpleMath::Matrix view)
{
	if (mp_game)
	{
		DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
		// デバッグ用オブジェクトの表示
		m_dbgObj->Draw(mp_game->GetContext(), *mp_game->GetState(), world, view, mp_game->GetProjection());
	}
}
void CollisionBox::DrawDebugCollision(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view)
{
	if (mp_game)
	{
		//DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
		// デバッグ用オブジェクトの表示
		m_dbgObj->Draw(mp_game->GetContext(), *mp_game->GetState(), world, view, mp_game->GetProjection());
	}
}
