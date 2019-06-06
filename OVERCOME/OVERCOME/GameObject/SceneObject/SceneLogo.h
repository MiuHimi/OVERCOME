//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

class MatrixManager;
class SceneLogo : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool                                               m_toTitleMoveOnChecker;        // タイトルシーンに進めるかどうかのチェック

	static const int                                   FADEOUT_NEED_SECOND;           // フェードアウトし始める秒数
	static const int                                   SCENE_CHANGE_NEED_SECOND;      // シーン遷移に必要な秒数

	float											   m_colorAlpha;				  // α値を変更

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureLogo;                // テクスチャハンドル(ロゴ)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;					  // スプライトバッチ

	MatrixManager*                                     mp_matrixManager;              // 行列管理変数

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneLogo(SceneManager* sceneManager);
	// デストラクタ
	~SceneLogo();

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update(DX::StepTimer const& timer) override;
	// 描画
	virtual void Render() override;
	// 終了
	virtual void Finalize() override;

};
