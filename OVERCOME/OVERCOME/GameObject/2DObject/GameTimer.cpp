//////////////////////////////////////////////////////////////
// File.    GameTimer.cpp
// Summary. GameTimerClass
// Date.    2018/10/16
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "GameTimer.h"

#include "../../Utility/DrawManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
GameTimer::GameTimer()
	: m_remainingTime(180)
	, m_timeNumWidth(40)
	, m_timeNumHeight(60)
	, m_timeUpFlag(false)
	, m_posBackground(DirectX::SimpleMath::Vector2(20.0f, 20.0f))
{
	// 数字の場所も背景と同じところで初期化しておく
	for (int i = 0; i < e_numDigit; ++i)
	{
		m_posCountDigit[i] = DirectX::SimpleMath::Vector2(m_posBackground);
	}

	// コモンステートの作成 
	m_states = std::make_unique<DirectX::CommonStates>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
}
/// <summary>
/// デストラクタ
/// </summary>
GameTimer::~GameTimer()
{
}

/// <summary>
/// 生成
/// </summary>
void GameTimer::Create()
{
	// テクスチャのロード
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_background_image.png", nullptr, m_textureBackground.GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_len_image.png", nullptr, m_textureNum.GetAddressOf());
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">起動経過時間</param>
bool GameTimer::Update(DX::StepTimer const& timer)
{
	static int countTime = 0;
	countTime++;

	// 60フレームで１秒経過
	if (countTime > 60)
	{
		m_remainingTime--;
		countTime = 0;
	}

	// 桁ごとの数字の設定
	m_queueDigit.push((m_remainingTime / 60) / 10);
	m_queueDigit.push((m_remainingTime / 60) % 10);
	m_queueDigit.push((m_remainingTime % 60) / 10);
	m_queueDigit.push((m_remainingTime % 60) % 10);

	// 制限時間が0になったらフラグを立てる
	if (m_remainingTime == 0)
	{
		m_timeUpFlag = true;
	}

	return true;
}

/// <summary>
/// 描画
/// </summary>
void GameTimer::Render()
{
	// 桁ごとの位置設定
	m_posCountDigit[e_minuteUpperDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 5, m_posBackground.y + 8);
	m_posCountDigit[e_minuteLowerDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 50, m_posBackground.y + 8);
	m_posCountDigit[e_secondUpperDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 110, m_posBackground.y + 8);
	m_posCountDigit[e_secondLowerDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 155, m_posBackground.y + 8);

	// スプライトの描画
	// 制限時間背景の描画
	DrawManager::SingletonGetInstance().Draw(m_textureBackground.Get(), m_posBackground);
	// 制限時間数列の描画
	for (int i = 0; i < e_numDigit; ++i)
	{
		// 切り取る数字を設定
		float u = m_timeNumWidth * m_queueDigit.front();
		m_queueDigit.pop();
		// 切り取る場所を設定
		RECT rect;
		rect.top    = LONG(0.0f);
		rect.left   = LONG(u);
		rect.right  = LONG(u + m_timeNumWidth);
		rect.bottom = LONG(m_timeNumHeight);
		
		//DrawManager::SingletonGetInstance().DrawRect(m_textureNum.Get(), m_posCountDigit[i], &rect);
	}

}
