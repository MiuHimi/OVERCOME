//////////////////////////////////////////////////////////////
// File.    GameScore.cpp
// Summary. GameScoreClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/DrawManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameScore::GameScore()
	: m_compareColum(0)
	, m_compareLine(0)
{	  
	GameScore::SetScore(0);
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
void GameScore::Create()
{
	// テクスチャのロード
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_0_image.png", nullptr, m_textureNum[0].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_1_image.png", nullptr, m_textureNum[1].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_2_image.png", nullptr, m_textureNum[2].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_3_image.png", nullptr, m_textureNum[3].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_4_image.png", nullptr, m_textureNum[4].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_5_image.png", nullptr, m_textureNum[5].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_6_image.png", nullptr, m_textureNum[6].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_7_image.png", nullptr, m_textureNum[7].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_8_image.png", nullptr, m_textureNum[8].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_9_image.png", nullptr, m_textureNum[9].GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameScore\\score_background_image.png", nullptr, m_textureBsckground.GetAddressOf());
	
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">起動経過時間</param>
bool GameScore::Update(DX::StepTimer const& timer)
{	
	// スコアの下限
	if (GameScore::GetScore() <= 0)
	{
		GameScore::SetScore(0);
	}
	
	return true;
}

/// <summary>
/// 描画
/// </summary>
void GameScore::Render()
{
	int hundredDigit = m_score / 100;
	int tenDigit = ((m_score % 100) / 10);
	int oneDigit = m_score % 10;

	// スプライトの描画
	DrawManager::SingletonGetInstance().Draw(m_textureBsckground.Get(), SimpleMath::Vector2(600.0f, 520.0f));

	DrawManager::SingletonGetInstance().Draw(m_textureNum[hundredDigit].Get(), DirectX::SimpleMath::Vector2(650.0f, 530.0f));
	DrawManager::SingletonGetInstance().Draw(m_textureNum[tenDigit].Get()    , DirectX::SimpleMath::Vector2(700.0f, 530.0f));
	DrawManager::SingletonGetInstance().Draw(m_textureNum[oneDigit].Get()    , DirectX::SimpleMath::Vector2(750.0f, 530.0f));
}

/// <summary>
/// ジャンプで乗り越えられたら加点
/// </summary>
/// <param name="j">行座標</param>
/// <param name="i">列座標</param>
void GameScore::AddPointChance(int j, int i)
{
	// 以前にいた座標と違えば
	if (j != m_compareColum || i != m_compareLine)
	{
		// 加点
		GameScore::FluctuationScore(10);
		// 座標再設定
		GameScore::SetAddPointChance(j, i);
	}
}
