//////////////////////////////////////////////////////////////
// File.    GameScore.h
// Summary. GameScoreClass
// Date.    2018/10/02
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <WICTextureLoader.h>
#include <CommonStates.h>

#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"

class GameScore
{
// メンバー変数
public:

private:
	static int                                         m_score;                // プレイヤーが得る得点

    // スコアの桁
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	DirectX::SimpleMath::Vector2					   m_scorePos;			   // スコアの表示位置
	DirectX::SimpleMath::Vector2					   m_scoreBGPos;		   // スコア背景の表示位置

	float											   m_scoreWidth;		   // スコアの幅
	float											   m_scoreHeight;		   // スコアの高さ
	float											   m_scoreBGWidth;		   // スコア背景の幅
	float											   m_scoreBGHeight;		   // スコア背景の高さ

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureBackground;   // テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_texture;             // テクスチャハンドル
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;			   // スプライトバッチ

// メンバー関数
public:
	// コンストラクタ
	GameScore();
	// デストラクタ
	~GameScore();

	// 生成
	void Create(const wchar_t* scoreFileName, const wchar_t* backgroundFileName);
	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render();

	//-----------------------------------Getter-----------------------------------//

	static int GetScore()                           { return m_score; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	static void SetScore(int score)                 { m_score = score; }
	//----------------------------------------------------------------------------//

	// 得点の増減
	void FluctuationScore(int addscore)				{ m_score += addscore; }

private:

};