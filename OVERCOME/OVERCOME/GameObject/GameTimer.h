//////////////////////////////////////////////////////////////
// File.    GameTimer.h
// Summary. GameTimerClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <queue>

#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

class GameTimer
{
// メンバー変数
public:

private:
	int                                                  m_remainingTime;               // 残り時間

	float                                                m_timeNumWidth;                // 数字の幅
	float                                                m_timeNumHeight;               // 数字の高さ

	bool                                                 m_timeUpFlag;                  // タイムアップになっているかどうか

	std::queue<int>                                      m_queueDigit;                  // 時間設定用
	
	enum CountDigit
	{
		e_minuteUpperDigit,
		e_minuteLowerDigit,
		e_secondUpperDigit,
		e_secondLowerDigit,

		e_numDigit
	};
	DirectX::SimpleMath::Vector2                         m_posBackground;               // タイマー背景位置
	DirectX::SimpleMath::Vector2                         m_posCountDigit[e_numDigit];   // タイマー数列位置

	std::unique_ptr<DirectX::SpriteBatch>                m_sprites;                     // スプライトバッチ
	std::unique_ptr<DirectX::CommonStates>               m_states;                      // コモンステート
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;           // テクスチャハンドル(背景)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureNum;                  // テクスチャハンドル(数字)

// メンバー関数
public:
	// コンストラクタ
	GameTimer();
	// デストラクタ
	~GameTimer();

	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render();

	// Getter,Setter
	// 時間取得、時間設定
	int GetTime()               { return m_remainingTime; }
	void SetTime(int time)      { m_remainingTime = time; }
	// 制限時間内(false)or制限時間到達(true)
	bool GetTimeUpFlag()        { return m_timeUpFlag; }

private:

};
