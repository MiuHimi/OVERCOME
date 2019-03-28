//////////////////////////////////////////////////////////////
// File.    GameFloor.h
// Summary. GameFloorClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"

#include "../../ExclusiveGameObject/CollisionBox.h"

class Game;
class MatrixManager;
class GameFloor : public CollisionBox
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelLattice;   // 格子柄床モデルハンドル
	//std::unique_ptr<CollisionBox> mp_floor;         // 床衝突判定用オブジェクト

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameFloor();
	// デストラクタ
	~GameFloor();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render(MatrixManager* matrixManager);
	// 廃棄処理
	void Depose();

private:
	void SetFogEffectDistance(float start, float end);

};