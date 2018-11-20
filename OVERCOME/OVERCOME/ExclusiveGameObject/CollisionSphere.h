/*
	CollisionSphere.h
	境界球を持ったObj3Dクラス
*/

#pragma once

#include "Obj3D.h"
#include "Collision.h"
#include "DebugSphere.h"

// 球衝突判定クラス
class CollisionSphere : public Obj3D
{
private:
	// 衝突判定情報
	Collision::Sphere m_collision;

	// 衝突判定表示デバッグオブジェクト
	std::unique_ptr<DebugSphere> m_dbgObj;

public:
	// Setter---
	// 衝突判定情報の設定
	void SetCollision(Collision::Sphere sphere);
	//---//

	// Getter---
	// 衝突判定情報の取得
	Collision::Sphere GetCollision();
	//---//

	// デバッグ用オブジェクト表示関数
	void DrawDebugCollision();

};