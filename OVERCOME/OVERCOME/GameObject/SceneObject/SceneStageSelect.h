//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2018/11/30
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "SceneBase.h"
#include <SpriteBatch.h>

#include "../ExclusiveGameObject/Collision2D.h"

class MatrixManager;
class SceneStageSelect : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toPlayMoveOnChecker;                   // プレイシーンに進めるかどうかのチェック
	bool m_returnToTitleChecker;                  // タイトルシーンに戻れるかどうかのチェック

	int selectedStage;                            // 選択されたステージ

	int m_selectSceneID;

	enum stage
	{
		ONE,
		TWO,

		NUM
	};

	const int                                            m_stageIconSize = 80;
	DirectX::SimpleMath::Vector2                         m_posStageIcon[NUM];      // タイマー数列位置
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureStageIcon[NUM];  // テクスチャハンドル(数字)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;      // テクスチャハンドル(数字)

	Collision2D                                          m_collideStageIcon[2];

	MatrixManager*                                       mp_matrixManager;      // 行列管理変数

	std::unique_ptr<DirectX::SpriteBatch>                mp_sprite;

	float                                                m_color;

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SceneStageSelect(SceneManager* sceneManager);
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

private:
	// 選択しているステージを読み込み
	void LoadStage(int stageID);

	// 選択しているステージを表示
	void ShowStage();
};
