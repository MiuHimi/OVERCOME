//////////////////////////////////////////////////////////////
// File.    InputManager.cpp
// Summary. InputManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <math.h>

#include "../pch.h"
#include "MathManager.h"

// usingディレクトリ


/// <summary>
/// コンストラクタ
/// </summary>
MathManager::MathManager()
{
}

/// <summary>
/// 二点間の距離を求めて返す(2D)
/// 平方根は求めない
/// </summary>
/// <param name="x">地点A:X座標</param>
/// <param name="y">地点A:Y座標</param>
/// <param name="x2">地点B:X座標</param>
/// <param name="y2">地点B:Y座標</param>
/// <returns>距離(float)</returns>
float MathManager::GetDistancePoints2D(float x, float y, float x2, float y2)
{
	float dist = (x2 - x) * (x2 - x) + (y2 - y) * (y2 - y);

	return dist;
}

/// <summary>
/// 二点間の距離を求めて返す(3D)
/// 平方根は求めない
/// </summary>
/// <param name="x">地点A:X座標</param>
/// <param name="y">地点A:Y座標</param>
/// <param name="z">地点A:Z座標</param>
/// <param name="x2">地点B:X座標</param>
/// <param name="y2">地点B:Y座標</param>
/// <param name="z2">地点B:Z座標</param>
/// <returns>距離(float)</returns>
float MathManager::GetDistancePoints3D(float x, float y, float z, float x2, float y2, float z2)
{
	float dist = ((x - x2)*(x - x2)) +
				 ((y - y2)*(y - y2)) +
				 ((z - z2)*(z - z2));

	return dist;
}
