//////////////////////////////////////////////////////////////
// File.    GameScore.cpp
// Summary. GameScoreClass
// Date.    2019/9/11
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "../2DObject/GameScore.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameScore::GameScore()
	: m_isFullScreen(false),
	  m_basePosX(0.0f),
	  mp_scoreStr(nullptr), mp_scoreBG(nullptr)
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
void GameScore::Create(const bool isFullScreen, const wchar_t* scoreFileName, const wchar_t* backgroundFileName)
{
	// 各変数の初期化
	m_score = 0;

	// フルスクリーンかどうか取得
	m_isFullScreen = isFullScreen;

	// アクティブなウィンドウのサイズ
	RECT activeWndRect;
	// アクティブなウィンドウのハンドルを取得
	HWND activeWnd = GetActiveWindow();
	// アクティブなウィンドウのハンドルからその画面の大きさを取得
	GetWindowRect(activeWnd, &activeWndRect);

	// ウィンドウのサイズを取得
	float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
	float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);
	// タイトルバーの高さを取得
	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);

	// ウィンドウの余分な部分
	float excessSpace = 16.0f;

	// スコア背景オブジェクトの生成
	mp_scoreBG = std::make_unique<Obj2D>();
	mp_scoreBG->Create(backgroundFileName, nullptr);
	mp_scoreBG->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 200.0f, 80.0f, 1.0f, 1.0f);
	if (m_isFullScreen)
	{
		mp_scoreBG->SetPos(SimpleMath::Vector2((activeWndRect.right - activeWndRect.left) - mp_scoreBG->GetWidth(),
											   (activeWndRect.bottom - activeWndRect.top) - mp_scoreBG->GetHeight()));
	}
	else
	{
		mp_scoreBG->SetPos(SimpleMath::Vector2(float(activeWndRect.right - activeWndRect.left - 20.0f) - mp_scoreBG->GetWidth(),
											   float(activeWndRect.bottom - (activeWndRect.top + titlebarHeight + 20.0f)) - mp_scoreBG->GetHeight()));
	}
	mp_scoreBG->SetRect(0.0f, 0.0f, mp_scoreBG->GetWidth(), mp_scoreBG->GetHeight());

	// スコアオブジェクトの生成
	mp_scoreStr = std::make_unique<Obj2D>();
	mp_scoreStr->Create(scoreFileName, nullptr);
	mp_scoreStr->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 40.0f, 60.0f, 1.0f, 1.0f);
	if (m_isFullScreen)
	{
		mp_scoreStr->SetPos(SimpleMath::Vector2(mp_scoreBG->GetPos().x + 10.0f,
												(activeWndRect.bottom - activeWndRect.top) - (10.0f + mp_scoreStr->GetHeight())));
	}
	else
	{
		mp_scoreStr->SetPos(SimpleMath::Vector2(mp_scoreBG->GetPos().x + 10.0f,
												float(activeWndRect.bottom - (activeWndRect.top + titlebarHeight+20.0f)) - float(10.0f + mp_scoreStr->GetHeight())));
	}
	mp_scoreStr->SetRect(0.0f, 0.0f, mp_scoreStr->GetWidth(), mp_scoreStr->GetHeight());

	// スコア表示基準位置設定
	m_basePosX = mp_scoreStr->GetPos().x;
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
	int thousandDigit = m_score / 1000;			 // 1000の位
	int hundredDigit = ((m_score / 100) % 10);   // 100の位
	int tenDigit = ((m_score % 100) / 10);		 // 10の位
	int oneDigit = m_score % 10;				 // 1の位

	// スコア背景の描画
	mp_scoreBG->Render();

	// スコアの描画
	for (int i = 0; i < Digit::NUM; i++)
	{
		// 表示場所設定
		float charSpace = 10.0f;
		mp_scoreStr->SetPos(SimpleMath::Vector2(m_basePosX + (i*charSpace)+(i*mp_scoreStr->GetWidth()),
												mp_scoreStr->GetPos().y));

		RECT rect;
		float scoreWidth = mp_scoreStr->GetWidth();
		float scoreHeight = mp_scoreStr->GetHeight();
		// 桁に応じて切り取る位置を変える
		switch (i)
		{
			// 1000の位
		case THOUSAND:
			rect = { LONG(scoreWidth * thousandDigit), LONG(0), LONG((scoreWidth * thousandDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// 100の位
		case HUNDRED:
			rect = { LONG(scoreWidth * hundredDigit), LONG(0), LONG((scoreWidth * hundredDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// 10の位
		case TEN:
			rect = { LONG(scoreWidth * tenDigit), 0, LONG((scoreWidth * tenDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// 1の位
		case ONE:
			rect = { LONG(scoreWidth * oneDigit), LONG(0), LONG((scoreWidth * oneDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// それ以外
		default:
			break;
		}
		mp_scoreStr->Render();
	}
}
