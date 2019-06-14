//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

class MatrixManager;
class SceneTitle : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool                                               m_toStageSelectMoveOnChecker;
																				  // ステージセレクトシーンに進めるかどうかのチェック

	float											   m_colorAlpha;			  // α値を変更

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureBackground;      // テクスチャハンドル(背景)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureTitle;		      // テクスチャハンドル(タイトル)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;				  // スプライトバッチ

	float											   m_titleWidth;			  // タイトルの幅
	float											   m_titleHeight;			  // タイトルの高さ
	DirectX::SimpleMath::Vector2					   m_TitlePos;				  // タイトルの表示位置

	MatrixManager*                                     mp_matrixManager;          // 行列管理変数

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneTitle(SceneManager* sceneManager);
	// デストラクタ
	~SceneTitle();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 終了
	virtual void Finalize() override;
};
