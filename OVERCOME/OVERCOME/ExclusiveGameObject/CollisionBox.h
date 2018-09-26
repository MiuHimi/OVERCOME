/*
	CollisionBox.h
	境界箱を持ったObj3Dクラス
*/

#pragma once

#include "Obj3D.h"
#include "Collision.h"
#include "DebugBox.h"

// 箱衝突判定クラス
class CollisionBox : public Obj3D
{
private:
	// 衝突判定情報
	Collision::Box m_collision;

	// 衝突判定表示デバッグオブジェクト
	std::unique_ptr<DebugBox> m_dbgObj;

public:
	// Setter---
	// 衝突判定情報の設定
	void SetCollision(Collision::Box box);
	//---//

	// Getter---
	// 衝突判定情報の取得
	Collision::Box GetCollision();
	//---//
	
	// デバッグ用オブジェクト表示関数
	void DrawDebugCollision(DirectX::SimpleMath::Matrix view);
	void DrawDebugCollision(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view);

};