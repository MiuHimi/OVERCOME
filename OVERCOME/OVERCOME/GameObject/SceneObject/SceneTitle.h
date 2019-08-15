//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2019/08/13
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../2DObject/Obj2D.h"
#include "../3DObject/GameCamera.h"

#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class SceneTitle : public SceneBase
{
// メンバー変数
//public:

private:
	bool                                               m_toStageSelectMoveOnChecker;
																				  // ステージセレクトシーンに進めるかどうかのチェック

	float											   m_fadeAlpha;				  // フェード用のα値
	float											   m_colorAlpha;			  // α値を変更

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureFade;			  // テクスチャハンドル(背景)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureTitle;		      // テクスチャハンドル(タイトル)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;				  // スプライトバッチ

	float											   m_titleWidth;			  // タイトルの幅
	float											   m_titleHeight;			  // タイトルの高さ
	DirectX::SimpleMath::Vector2					   m_TitlePos;				  // タイトルの表示位置

	std::unique_ptr<Obj2D>							   mp_startBtn;				  // スタートボタンオブジェクト

	float											   m_fadeImageWidth;		  // フェード画像の幅
	float											   m_fadeImageHeight;		  // フェード画像の高さ
	DirectX::SimpleMath::Vector2					   m_fadeImagePos;			  // フェード画像の表示位置

	std::unique_ptr<GameCamera>						   mp_camera;			      // ゲームカメラオブジェクト

	std::unique_ptr<DirectX::Model>					   mp_modelHouse;			  // 家モデルオブジェクト

	MatrixManager*                                     mp_matrixManager;          // 行列管理変数

	EffectManager*									   mp_effectManager;		  // エフェクト管理変数

// メンバー関数
public:
	// コンストラクタ
	SceneTitle(SceneManager* sceneManager, bool isFullScreen);
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

//private:

};
