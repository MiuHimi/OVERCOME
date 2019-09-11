//////////////////////////////////////////////////////////////
// File.    GameScore.h
// Summary. GameScoreClass
// Date.    2019/9/11
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <WICTextureLoader.h>
#include <CommonStates.h>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"
#include "Obj2D.h"

class GameScore
{
// メンバー変数
//public:

private:
	static int								m_score;			// プレイヤーが得る得点

    // スコアの桁
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	bool									m_isFullScreen;		// フルスクリーンかどうか

	float									m_basePosX;			// スコア表示基準位置

	std::unique_ptr<Obj2D>					mp_scoreStr;		// スコアオブジェクト
	std::unique_ptr<Obj2D>					mp_scoreBG;			// スコア背景オブジェクト

// メンバー関数
public:
	// コンストラクタ
	GameScore();
	// デストラクタ
	~GameScore();

	// 生成
	void Create(const bool isFullScreen, const wchar_t* scoreFileName, const wchar_t* backgroundFileName);
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

//private:

};