//////////////////////////////////////////////////////////////
// File.    FPS.h
// Summary. ControlFPSClass
// Date.    2020/01/10
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

class FPS
{
// メンバー変数
//public:

private:
	int					m_startTime = 0;	// 測定開始時刻
	int					m_flameCount = 0;	// フレームカウンター
	float				m_fps = 0;			// FPS計測変数
	static const int	SAMPLE_FPS = 60;	// 平均を取るサンプル数
	static const int	SET_FPS = 60;		// 設定したいFPS

// メンバー変数
public:
	// コンストラクタ
	FPS();
	// デストラクタ
	~FPS();

	// FPS計測用更新関数
	bool FPSUpdate();

	// FPS制御用Sleep関数
	void FPSWait();

	// FPSを取得
	float GetFPS() { return m_fps; }

//private:

};