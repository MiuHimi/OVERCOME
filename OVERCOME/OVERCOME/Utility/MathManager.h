//////////////////////////////////////////////////////////////
// File.    MathManager.h
// Summary. MathManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"

#include "SingletonBase.h"

class MathManager : public SingletonBase<MathManager>
{
// メンバー変数
//public:

//private:

// メンバー関数
private:
	// フレンド関数
	friend class SingletonBase<MathManager>;

	// コンストラクタ
	MathManager();

public:
	// 二点間の距離を求めて返す(二次元)
	float GetDistancePoints2D(float x, float y, float x2, float y2);

	// 二点間の距離を求めて返す(三次元)
	float GetDistancePoints3D(float x, float y, float x2, float y2, float x3, float y3);

};