/*
	Obj3D.h
*/

#pragma once

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../pch.h"

class Game;
class MatrixManager;
class Obj3D
{
// メンバー変数
protected:
	// モデルハンドル
	DirectX::Model* mp_model;

	// 位置
	DirectX::SimpleMath::Vector3 m_position;
	// 回転
	DirectX::SimpleMath::Quaternion m_rotation;

	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;

// メンバー関数
public:
	// コンストラクタ
	Obj3D();
	// デストラクタ
	~Obj3D();

	// 更新
	virtual bool Update(float elapsedTime);
	// 描画
	virtual void Render(MatrixManager* matrixManager);


	// Setter
	//---
	// モデルハンドルを設定
	void SetModel(DirectX::Model* model)                             { mp_model = model; }

	// 位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector3 position)    { m_position = position; }
	// 回転を設定
	void SetRotation(const DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	//---//

};