//////////////////////////////////////////////////////////////
// File.    CollisionBox.h
// Summary. 境界箱を持ったObj3Dクラス
// Date.    2018/10/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../GameObject/3DObject/Obj3D.h"
#include "Collision.h"
#include "DebugBox.h"

// 箱衝突判定クラス
class MatrixManager;
class CollisionBox : public Obj3D
{
// メンバー変数
public:

private:
	Collision::Box                m_collision;     // 衝突判定情報

	std::unique_ptr<DebugBox>     m_dbgObj;        // 衝突判定表示デバッグオブジェクト

// メンバー関数
public:
	// デバッグ用オブジェクト表示関数
	void DrawDebugCollision(MatrixManager* matrixManager);

	// 衝突判定情報の設定
	void SetCollision(Collision::Box box);
	// 衝突判定情報の取得
	Collision::Box GetCollision();

private:

};