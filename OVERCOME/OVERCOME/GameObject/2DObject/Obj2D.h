//////////////////////////////////////////////////////////////
// File.    Obj2D.h
// Summary. Obj2DClass,BaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class Obj2D
{
// メンバー変数
protected:
	// 位置
	DirectX::SimpleMath::Vector2 m_position;

	// 幅、高さ
	float m_width, m_height;

// メンバー関数
public:
	// コンストラクタ
	Obj2D();
	// デストラクタ
	~Obj2D();

	// 更新
	virtual bool Update(float elapsedTime);
	// 描画
	virtual void Render();

	//-----------------------------------Setter-----------------------------------//

	// 位置を設定
	void SetPosition(const DirectX::SimpleMath::Vector2 position) { m_position = position; }
	// 幅、高さを設定
	void SetWidthHeight(const float width, const float height)    { m_width = width, m_height = height; }
	//----------------------------------------------------------------------------//

};