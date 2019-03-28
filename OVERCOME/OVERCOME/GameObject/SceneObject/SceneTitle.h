//////////////////////////////////////////////////////////////
// File.    SceneTitle.h
// Summary. SceneTitleClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

//#include "../../ExclusiveGameObject/EffectManager.h"

class MatrixManager;
class SceneTitle : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool              m_toPlayMoveOnChecker;              // ロゴシーンに進めるかどうかのチェック

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureBackground;   // テクスチャハンドル(背景)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureTitle;        // テクスチャハンドル(タイトル)

	//EffectManager*	                                   mp_effectManager;      // エフェクト管理変数
	MatrixManager*                                     mp_matrixManager;      // 行列管理変数

	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;

	float                                              m_color;

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
	// 描画
	virtual void Finalize() override;
};
