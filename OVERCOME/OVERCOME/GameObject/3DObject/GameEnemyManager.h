//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.h
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"

#include <vector>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "GameEnemy.h"

#include "../../ExclusiveGameObject/CollisionSphere.h"

class GameEnemy;
class MatrixManager;
class Player;
class GameEnemyManager
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	static const int             m_maxEnemyNum = 100;         // 最大敵数
	static const int             m_maxAliveDist = 50;

	static const int             m_needRespawnTime = 90;      // リスポーンに必要な時間(フレーム)
	int                          m_respawnTime;               // リスポーン時間(フレーム)

	GameEnemy*                   mp_enemy[m_maxEnemyNum];
	Player*                      mp_player;

	// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameEnemyManager();
	// デストラクタ
	~GameEnemyManager();

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

	//-----------------------------------Getter-----------------------------------//

	int GetMaxEnemyNum()                       { return m_maxEnemyNum; }
	DirectX::SimpleMath::Vector3 GetPos(int i) { return mp_enemy[i]->GetPos(); }
	bool GetEnemyState(int i)				   { return mp_enemy[i]->GetState(); }
	Collision::Sphere GetEnemyCollide(int i)   { return mp_enemy[i]->GetCollision(); }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	void SetEnemyState(int i, bool flag) {mp_enemy[i]->SetState(flag); }
	//----------------------------------------------------------------------------//

private:


};