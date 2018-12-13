//////////////////////////////////////////////////////////////
// File.    GameRoad.h
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class GameRoad : public CollisionBox 
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:
	enum PosType
	{
		START,
		GOAL,

		NUM
	};

private:
	std::unique_ptr<DirectX::Model> m_modelRoadStraight;   // 直線型道路
	std::unique_ptr<DirectX::Model> m_modelRoadStop;       // 末端型道路
	std::unique_ptr<DirectX::Model> m_modelRoadCurve;      // 曲線型道路
	std::unique_ptr<DirectX::Model> m_modelRoadBranch;     // 分岐型道路
	//std::unique_ptr<CollisionBox> mp_floor;              // 道路衝突判定用オブジェクト

	static const int m_maxFloorWidth = 100;                // フロアの最大幅(設定[m])
	static const int m_maxFloorHeight = 100;               // フロアの最大幅(設定[m])

	static const int m_maxFloorBlock = 20;                 // ブロックごとに分けたフロアの幅(個数)
	const int m_roadBlockSize = 5;                         // 単位ブロックの幅(設定[m])

	
	DirectX::SimpleMath::Vector2 m_posStartEnd[2];         // スタート位置とゴールの位置を記憶

	struct road
	{
		int roadType;                                      // 道路の種類
		DirectX::SimpleMath::Vector3 pos;                  // 道路の位置
		int rotaAngle;                                     // 回転角を設定( (0 or 1 or 2 or 3) * 90.0f )
	};

	road m_roadObject[m_maxFloorBlock][m_maxFloorBlock];   // 道路の情報を格納
	CollisionBox* mp_roadCollideObject[m_maxFloorBlock][m_maxFloorBlock];

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
	void Render();
	// 廃棄処理
	void Depose();

	int GetMaxFloorBlock() { return m_maxFloorBlock; }

	DirectX::SimpleMath::Vector2 GetPosType(int posType) { return m_posStartEnd[posType]; }

	road GetRoadObject(int j, int i) { return m_roadObject[j][i]; }
	CollisionBox* GetCollisionObject(int j, int i) { return mp_roadCollideObject[j][i]; }


private:
	void SetFogEffectDistance(float start, float end);

};