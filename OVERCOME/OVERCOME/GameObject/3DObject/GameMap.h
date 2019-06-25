//////////////////////////////////////////////////////////////
// File.    GameMap.h
// Summary. GameMapClass
// Date.    2019/03/25
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../ExclusiveGameObject/CollisionMesh.h"

class MatrixManager;
class Player;
class GameMap
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	std::unique_ptr<DirectX::Model> mp_modelMap;	   // マップモデル

	std::unique_ptr<CollisionMesh>  mp_collisionStage; // コリジョンメッシュ

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameMap();
	// デストラクタ
	~GameMap();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer, Player* player);
	// 描画
	void Render(MatrixManager* matrixManager);
	// 廃棄処理
	void Depose();

private:
	void SetFogEffectDistance(float start, float end);

};