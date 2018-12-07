//////////////////////////////////////////////////////////////
// File.    GameTarget.h
// Summary. GameTargetClass
// Date.    2018/11/19
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class Game;
class GameTarget : public CollisionBox
{
	// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelTarget;   // 直線型道路

	static const int m_maxFloorWidth = 100;                // フロアの最大幅(設定[m])
	static const int m_maxFloorHeight = 100;               // フロアの最大幅(設定[m])

	static const int m_maxFloorBlock = 20;                 // ブロックごとに分けたフロアの幅(個数)
	const int m_roadBlockSize = 5;                         // 単位ブロックの幅(設定[m])

	int m_stageCount;

	const float m_targetRadius = 1.0f;

	struct target
	{
		DirectX::SimpleMath::Vector3       pos;           // 道路の位置
		int                                height;        // 的の高さ
		bool                               state;         // 生存フラグ
		int                                rotaAngle;     // 回転角を設定( (0 or 1 or 2 or 3) * 90.0f )
	};

	target m_targetObject[m_maxFloorBlock][m_maxFloorBlock];   // 道路の情報を格納
	CollisionBox* mp_targetCollideObject[m_maxFloorBlock][m_maxFloorBlock];

	// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameTarget();
	// デストラクタ
	~GameTarget();

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

	bool GetState(int j, int i) { return m_targetObject[j][i].state; }
	void SetState(int j, int i, bool state) { m_targetObject[j][i].state = state; }
	target GetTargetObject(int j, int i) { return m_targetObject[j][i]; }
	CollisionBox* GetCollisionObject(int j, int i) { return mp_targetCollideObject[j][i]; }


private:


};