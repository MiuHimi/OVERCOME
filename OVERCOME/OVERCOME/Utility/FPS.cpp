//////////////////////////////////////////////////////////////
// File.    FPS.cpp
// Summary. ControlFPSClass
// Date.    2020/01/10
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "FPS.h"

/// <summary>
/// コンストラクタ
/// </summary>
FPS::FPS() : m_startTime(0),m_flameCount(0),m_fps(0)
{
}
/// <summary>
/// デストラクタ
/// </summary>
FPS::~FPS()
{
}

/// <summary>
/// FPS計測用更新関数
/// </summary>
/// <returns></returns>
bool FPS::FPSUpdate()
{
	//1フレーム目なら時刻を記憶
	if (m_flameCount == 0) m_startTime = GetCurrentTime();

	if (m_flameCount == SAMPLE_FPS)
	{
		//60フレーム目なら平均を計算する
		int t = GetCurrentTime();
		m_fps = 1000.f / ((t - m_startTime) / (float)SAMPLE_FPS);
		m_flameCount = 0;
		m_startTime = t;
	}

	// カウント更新
	m_flameCount++;
	return true;
}

/// <summary>
/// FPS制御用Sleep関数
/// </summary>
void FPS::FPSWait()
{
	// かかった時間を計測
	int tookTime = GetCurrentTime() - m_startTime;
	// そこから待つべき時間を算出
	int waitTime = m_flameCount * 1000 / SET_FPS - tookTime;
	// 算出した分だけ待機
	if (waitTime > 0) Sleep(waitTime);
}
