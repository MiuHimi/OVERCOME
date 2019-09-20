//////////////////////////////////////////////////////////////
// File.    SceneResult.h
// Summary. SceneResultClass
// Date.    2019/09/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"

#include "../2DObject/Obj2D.h"

class GameScore;
class MatrixManager;
class SceneResult : public SceneBase
{
// メンバー変数
//public:

private:
	bool								m_toTitleMoveOnChecker;		// タイトルシーンに進めるかどうかのチェック
	bool								m_returnToPlayChecker;		// プレイシーンに戻れるかどうかのチェック
	bool								m_resultState;				// リザルトシーンの状態

	static const int					SCORE_SIZE;					// スコアの数字のサイズ

	// スコアの桁
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	DirectX::SimpleMath::Vector2		m_scoreBasePos;				// スコアの表示基準位置

	std::unique_ptr<GameScore>			mp_gameScore;				// スコアのポインタ

	std::unique_ptr<Obj2D>				mp_fade;					// フェード画像オブジェクト
	std::unique_ptr<Obj2D>				mp_score;					// スコアオブジェクト
	std::unique_ptr<Obj2D>				mp_bg;						// 背景オブジェクト
	std::unique_ptr<Obj2D>				mp_resultStr;				// RESULT文字列オブジェクト

	std::unique_ptr<Obj2D>				mp_gameover;				// ゲームオーバー画像オブジェクト
	std::unique_ptr<Obj2D>				mp_gameoverBG;				// ゲームオーバー背景オブジェクト

	MatrixManager*						mp_matrixManager;			// 行列管理変数

// メンバー関数
public:
	// コンストラクタ
	SceneResult(SceneManager* sceneManager, bool isFullScreen);
	// デストラクタ
	~SceneResult();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 終了
	virtual void Finalize() override;

//private:

};
