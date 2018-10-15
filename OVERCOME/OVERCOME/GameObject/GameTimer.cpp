//////////////////////////////////////////////////////////////
// File.    GameTimer.cpp
// Summary. GameTimerClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "GameTimer.h"


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
	for (int i = 0; i < e_numDigit; ++i)
	{
		m_posCountDigit[i] = DirectX::SimpleMath::Vector2(m_posBackground);
	}

	// スプライトバッチの作成
	m_sprites = std::make_unique<DirectX::SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());
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
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());

	// 制限時間背景の描画
	m_sprites->Draw(m_textureBackground.Get(), m_posBackground);
	// 制限時間数列の描画
	for (int i = 0; i < e_numDigit; ++i)
	{
		// 切り取る位置を設定
		float u = m_timeNumWidth * m_queueDigit.front();
		m_queueDigit.pop();
		RECT rect;
		rect.top = LONG(0.0f);
		rect.left = LONG(u);
		rect.right = LONG(u + m_timeNumWidth);
		rect.bottom = LONG(m_timeNumHeight);
		
		m_sprites->Draw(m_textureNum.Get(), m_posCountDigit[i], &rect, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f),1, DirectX::SpriteEffects_None, 0.0f);
	}

	m_sprites->End();
}
