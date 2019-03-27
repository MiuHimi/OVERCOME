//////////////////////////////////////////////////////////////
// File.    GameEnemyManager.cpp
// Summary. GameEnemyManagerClass
// Date.    2019/03/26
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include <time.h>
#include "GameEnemyManager.h"
#include "Player.h"

#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

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
		if (!mp_enemy[i]->GetState())
		{
			if (m_needRespawnTime < m_respawnTime && player->GetPlaying())
			{
				// まだ出現出来たら出現準備
				mp_enemy[i]->SetState(true);
				mp_enemy[i]->SetPos(Vector3(-35.0f, 5.0f, -25.5f));
				mp_enemy[i]->SetVel(Vector3(float(rand() % 5) / 20.0f, float(rand() % 2 - 1) / 10.0f, float(rand() % 5) / 20.0f));
				m_respawnTime = 0;
				break;
			}
		}
	}

	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		// 敵の動き
		if (mp_enemy[i]->GetState())
		{
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
