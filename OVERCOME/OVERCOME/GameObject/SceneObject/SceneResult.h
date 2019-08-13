//////////////////////////////////////////////////////////////
// File.    SceneResult.h
// Summary. SceneResultClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"

class GameScore;
class MatrixManager;
class SceneResult : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
//public:

private:
	bool												 m_toTitleMoveOnChecker;  // タイトルシーンに進めるかどうかのチェック
	bool												 m_returnToPlayChecker;   // プレイシーンに戻れるかどうかのチェック
	bool												 m_resultState;			  // リザルトシーンの状態

	static const int									 SCORE_SIZE;			  // スコアの数字のサイズ

	// スコアの桁
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	DirectX::SimpleMath::Vector2						 m_scorePos;			  // スコアの表示位置

	std::unique_ptr<GameScore>							 mp_gameScore;			  // スコアのポインタ

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     mp_textureScore;         // テクスチャハンドル(スコア)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     mp_textureBackground;    // テクスチャハンドル(背景)
	std::unique_ptr<DirectX::SpriteBatch>                mp_sprite;				  // スプライトバッチ

	MatrixManager*                                       mp_matrixManager;        // 行列管理変数

// メンバー関数(関数、Getter、Setter)
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
