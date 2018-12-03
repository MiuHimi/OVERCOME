//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.h
// Summary. SceneStageSelectClass
// Date.    2018/11/30
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <vector>
#include <SpriteFont.h>
#include "SceneBase.h"

#include "../GameCamera.h"

class SceneStageSelect : public SceneBase
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	bool m_toPlayMoveOnChecker;                   // プレイシーンに進めるかどうかのチェック
	bool m_returnToTitleChecker;                  // タイトルシーンに戻れるかどうかのチェック

	std::unique_ptr<DirectX::Model> m_modelRoadStraight;   // 直線型道路
	std::unique_ptr<DirectX::Model> m_modelRoadStop;       // 末端型道路
	std::unique_ptr<DirectX::Model> m_modelRoadCurve;      // 曲線型道路
	std::unique_ptr<DirectX::Model> m_modelRoadBranch;     // 分岐型道路

	static const int m_maxFloorWidth = 100;                // フロアの最大幅(設定[m])
	static const int m_maxFloorHeight = 100;               // フロアの最大幅(設定[m])

	static const int m_maxFloorBlock = 20;                 // ブロックごとに分けたフロアの幅(個数)
	const int m_roadBlockSize = 5;                         // 単位ブロックの幅(設定[m])

	struct road
	{
		int roadType;                                      // 道路の種類
		DirectX::SimpleMath::Vector3 pos;                  // 道路の位置
		int rotaAngle;                                     // 回転角を設定( (0 or 1 or 2 or 3) * 90.0f )
	};

	//road m_roadObject[m_maxFloorBlock][m_maxFloorBlock];   // 道路の情報を格納
	std::vector<std::vector<road>> m_roadObject;

	std::unique_ptr<GameCamera> mp_camera;        // カメラオブジェクト

	bool m_showFlag;                                        // 
	int m_selectSceneID;

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
