//////////////////////////////////////////////////////////////
// File.    CountManager.h
// Summary. CountManagerClass(Frame Count)
// Date.    2018/11/20
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/SingletonBase.h"

class CountManager : public SingletonBase<CountManager>
{
// メンバー変数
public:
	int           m_count;         // カウント(フレーム数)
	bool          m_countFlag;     // カウントが開始されたらフラグが立つ
	const int     m_fps = 60;      // 60フレームで一秒

private:

// メンバー関数
public:
	// デストラクタ
	~CountManager();

	// 更新
	void Update();

	// フレンド関数
	friend SingletonBase<CountManager>;

	// カウントを開始
	void CountStart();
	// カウントしていく
	void Counting();
	// カウントをリセット
	void CountReset();

	// カウント(フレーム数)を取得
	int GetCount();
	// FPSを取得
	int GetFPS();

private:
	// コンストラクタ
	CountManager();

};