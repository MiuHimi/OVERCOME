//////////////////////////////////////////////////////////////
// File.    GameScore.cpp
// Summary. GameScoreClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/CommonStateManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameScore::GameScore()
	: m_scorePos(0.0f, 0.0f),
	  m_scoreBGPos(0.0f, 0.0f),
	  m_scoreWidth(0.0f), m_scoreHeight(0.0f),
	  m_scoreBGWidth(0.0f), m_scoreBGHeight(0.0f),
	  mp_texture(nullptr),
	  mp_textureBackground(nullptr),
	  mp_sprite(nullptr)
{	  
}
/// <summary>
/// デストラクタ
/// </summary>
GameScore::~GameScore()
{
}

/// <summary>
/// 生成
/// </summary>
void GameScore::Create(const wchar_t* scoreFileName, const wchar_t* backgroundFileName)
{
	// 各変数の初期化
	m_score = 0;

	m_scorePos = SimpleMath::Vector2(650.0f, 560.0f);
	m_scoreBGPos = SimpleMath::Vector2(700.0f, 560.0f);

	m_scoreWidth = 40.0f; 
	m_scoreHeight = 60.0f;
	m_scoreBGWidth = 200.0f;
	m_scoreBGHeight = 80.0f;

	// テクスチャのロード
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), scoreFileName, nullptr, mp_texture.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), backgroundFileName, nullptr, mp_textureBackground.GetAddressOf());
	
	// スプライトバッチの初期化
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">起動経過時間</param>
bool GameScore::Update(DX::StepTimer const& timer)
{	
	// スコアの下限
	if (GetScore() <= 0)
	{
		SetScore(0);
	}
	
	return true;
}

/// <summary>
/// 描画
/// </summary>
void GameScore::Render()
{
	// スコア
	int hundredDigit = m_score / 100;		// 100の位
	int tenDigit = ((m_score % 100) / 10);  // 10の位
	int oneDigit = m_score % 10;			// 1の位

	// スコアの描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// 背景
	RECT rectBG = { 0, 0, 200, 80 };
	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(m_scoreBGPos.x - m_scoreBGWidth / 2.0f, m_scoreBGPos.y - m_scoreBGHeight / 2.0f), &rectBG, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// スコア
	for (int i = 0; i < Digit::NUM; i++)
	{
		RECT rect;
		// 桁に応じて切り取る位置を変える
		switch (i)
		{
		// 100の位
		case HUNDRED:
			rect = { int(m_scoreWidth) * hundredDigit, 0, int(m_scoreWidth) * hundredDigit + int(m_scoreWidth), int(m_scoreHeight) };
			break;
		// 10の位
		case TEN:
			rect = { int(m_scoreWidth) * tenDigit, 0, int(m_scoreWidth) * tenDigit + int(m_scoreWidth), int(m_scoreHeight) };
			break;
		// 1の位
		case ONE:
			rect = { int(m_scoreWidth) * oneDigit, 0, int(m_scoreWidth) * oneDigit + int(m_scoreWidth), int(m_scoreHeight) };
			break;
		// それ以外の位
		default:
			break;
		}

		float indent = 0;
		if (i == 0)
		{
			indent = 0;
		}
		else
		{
			indent = 10;
		}
		mp_sprite->Draw(mp_texture.Get(), SimpleMath::Vector2(m_scorePos.x + float(i * m_scoreWidth) + indent, m_scorePos.y - (float)m_scoreHeight / 2.0f), &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
}
