//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2019/06/06
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../../ExclusiveGameObject/Collision2D.h"

class MatrixManager;
class SceneStageSelect : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
//public:

private:
	bool                                               m_toPlayMoveOnChecker;     // タイトルシーンに進めるかどうかのチェック

	int												   m_selectedStage;			  // 選択されたステージ

	// ステージ数
	enum stage
	{
		ONE,
		TWO,

		NUM
	};

	float											   m_colorAlpha;			  // α値を変更

	static const int                                   STAGE_ICON_SIZE;			  // ステージアイコンサイズ
	DirectX::SimpleMath::Vector2                       m_posStageIcon[NUM];       // ステージアイコン位置
	Collision2D                                        m_collideStageIcon[NUM];	  // ステージアイコン衝突判定
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureStageIcon[NUM];  // テクスチャハンドル(数字)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureBackground;      // テクスチャハンドル(数字)
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;				  // スプライトバッチ

	MatrixManager*                                     mp_matrixManager;		  // 行列管理変数

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneStageSelect(SceneManager* sceneManager, bool isFullScreen);
	// デストラクタ
	~SceneStageSelect();

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
