//////////////////////////////////////////////////////////////
// File.    GameRoad.h
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameRoad
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:
	enum PosType
	{
		START,
		GOAL,
		WAY,

		NUM
	};

private:
	static const int m_maxFloorWidth = 100;                // フロアの最大幅(設定[m])
	static const int m_maxFloorHeight = 100;               // フロアの最大幅(設定[m])

	static const int m_maxFloorBlock = 20;                 // ブロックごとに分けたフロアの幅(個数)
	const int m_roadBlockSize = 5;                         // 単位ブロックの幅(設定[m])

	DirectX::SimpleMath::Vector2 m_posStartEnd[2];         // スタート位置とゴールの位置を記憶

	struct road
	{
		int roadType;					  // 道の種類
		int roadNum;					  // 道の数(カウント用)
		DirectX::SimpleMath::Vector3 pos; // 位置   
	};

	road m_road[m_maxFloorBlock][m_maxFloorBlock];   // 道の情報を格納

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameRoad();
	// デストラクタ
	~GameRoad();

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

	int GetMaxFloorBlock() { return m_maxFloorBlock; }

	DirectX::SimpleMath::Vector2 GetPosType(int posType) { return m_posStartEnd[posType]; }

	road GetRoadObject(int j, int i) { return m_road[j][i]; }

private:
	void SetFogEffectDistance(float start, float end);

};