//////////////////////////////////////////////////////////////
// File.    SceneLogo.h
// Summary. SceneLogoClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteFont.h>
#include "SceneBase.h"

class SceneLogo : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool                                               m_toTitleMoveOnChecker;  // タイトルシーンに進めるかどうかのチェック

	int                                                m_fadeoutNeedTime;       // フェードアウトし始める秒数
	int                                                m_changeSceneNeedTime;   // シーン遷移に必要な秒数

	float                                              fadeoutCount;            // α値を変更

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureBackground;     // テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_textureLogo;           // テクスチャハンドル

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
