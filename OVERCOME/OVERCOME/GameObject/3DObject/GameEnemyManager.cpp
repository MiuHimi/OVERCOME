//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include <math.h>
#include <time.h>
#include "GameEnemyManager.h"
#include "Player.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// コンストラクタ
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_respawnTime(0),
	  mp_player(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameEnemyManager::~GameEnemyManager()
{
	// 敵オブジェクト削除
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		delete mp_enemy[i];
		mp_enemy[i] = nullptr;
	}

	// プレイヤーオブジェクト削除
	delete mp_player;
	mp_player = nullptr;
}

/// <summary>
/// 初期化
/// </summary>
void GameEnemyManager::Initialize()
{
	// 乱数初期化
	srand((unsigned int)time(NULL));
}
/// <summary>
/// 生成
/// </summary>
void GameEnemyManager::Create()
{
	// 敵の初期化、生成
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		mp_enemy[i] = new GameEnemy(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		mp_enemy[i]->Create();
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <returns>終了状態</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, Player* player)
{
	m_respawnTime++;

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// 敵の襲撃時間だったら更新する
		if (!player->GetEnemyTime())break;
		if (!mp_enemy[i]->GetState())
		{
			if (m_needRespawnTime < m_respawnTime && player->GetPlaying())
			{
				// まだ出現出来たら出現準備
				mp_enemy[i]->SetState(true);

				// カウントしなおし
				m_respawnTime = 0;

				// 場所設定
				switch (player->GetAssaultPoint())
				{
				case 1: mp_enemy[i]->SetPos(Vector3(-35.0f, 5.0f, -25.5f + float(rand() % 20 - 10))); break;
				case 2: mp_enemy[i]->SetPos(Vector3(0.0f, 15.0f, 25.5f + float(rand() % 20 - 10))); break;
				case 3: mp_enemy[i]->SetPos(Vector3(0.0f + float(rand() % 20 - 10), 3.0f, 30.0f)); break;
				case 4: mp_enemy[i]->SetPos(Vector3(-35.0f + float(rand() % 20 - 10), 5.0f, -25.5f + float(rand() % 10 - 5))); break;
				}

				// プレイヤーとの位置の差分
				SimpleMath::Vector3 enemyPosDiff = SimpleMath::Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x), float(player->GetPos().y - mp_enemy[i]->GetPos().y), float(player->GetPos().z - mp_enemy[i]->GetPos().z));
				// 敵の向き
				SimpleMath::Vector3 enemyDir = SimpleMath::Vector3(mp_enemy[i]->GetDir());

				{
					// ベクトルの長さを求める
					double lengthA = pow((enemyPosDiff.x * enemyPosDiff.x) + (enemyPosDiff.z * enemyPosDiff.z), 0.5);
					double lengthB = pow((enemyDir.x * enemyDir.x) + (enemyDir.z * enemyDir.z), 0.5);
					// 内積とベクトルの長さを使ってcosθを求める
					double cos_sita = enemyPosDiff.x * enemyDir.x + enemyPosDiff.z * enemyDir.z / (lengthA * lengthB);

					// cosθからθを求める
					double sita = acos(cos_sita);
					// デグリーで求める
					//sita = sita * 180.0 / double(XM_PI);

					mp_enemy[i]->SetRotateY(float(sita));
				}

				//{
				//	// ベクトルの長さを求める
				//	double lengthA = pow((enemyPosDiff.y * enemyPosDiff.y) + (enemyPosDiff.z * enemyPosDiff.z), 0.5);
				//	double lengthB = pow((enemyDir.y * enemyDir.y) + (enemyDir.z * enemyDir.z), 0.5);
				//	// 内積とベクトルの長さを使ってcosθを求める
				//	double cos_sita = enemyPosDiff.y * enemyDir.y + enemyPosDiff.z * enemyDir.z / (lengthA * lengthB);
				//	// cosθからθを求める
				//	double sita = acos(cos_sita);
				//	mp_enemy[i]->SetRotateX(float(sita));
				//}
			}
		}
	}

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// 敵の襲撃時間だったら更新する
		if (!player->GetEnemyTime())break;
		// 敵の動き
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->SetVel(Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x) / 20.0f, 
										float(player->GetPos().y - mp_enemy[i]->GetPos().y) / 20.0f, 
										float(player->GetPos().z - mp_enemy[i]->GetPos().z) / 20.0f));
			mp_enemy[i]->Update(timer);

			// マップの中心から50ｍ離れたら消える
			float dist = (mp_enemy[i]->GetPos().x - 0.0f)*(mp_enemy[i]->GetPos().x - 0.0f) +
						 (mp_enemy[i]->GetPos().y - 0.0f)*(mp_enemy[i]->GetPos().y - 0.0f) +
						 (mp_enemy[i]->GetPos().z - 0.0f)*(mp_enemy[i]->GetPos().z - 0.0f);
			if (m_maxAliveDist * m_maxAliveDist < dist)
			{
				mp_enemy[i]->SetState(false);
			}
		}
	}

	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameEnemyManager::Render(MatrixManager* matrixManager)
{
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->Render(matrixManager);
		}
	}
}

/// <summary>
/// 廃棄
/// </summary>
void GameEnemyManager::Depose()
{
}
