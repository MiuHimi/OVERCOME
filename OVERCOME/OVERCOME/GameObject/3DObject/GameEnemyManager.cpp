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
#include "../../Utility/DrawManager.h"

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// コンストラクタ
/// </summary>
GameEnemyManager::GameEnemyManager()
	: m_respawnTime(0), m_dengerousDirFB(DIRECTION::NONE), m_dengerousDirLR(DIRECTION::NONE), m_lengthTmp(0),m_assault(false),
	  mp_player(nullptr) ,m_textureDengerousH(nullptr), m_textureDengerousV(nullptr)
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
	mp_gameCamera = std::make_unique<GameCamera>(DX::DeviceResources::SingletonGetInstance().GetOutputSize().right, DX::DeviceResources::SingletonGetInstance().GetOutputSize().bottom);
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

	// 敵の初期化、生成
	for (int i = 0; i < m_maxEnemyNum; i++)
	{
		m_compereLength[i] = 0.0;
	}

	// 危険サインの設定
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\dangerous_signH.png", nullptr, m_textureDengerousH.GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\dangerous_signV.png", nullptr, m_textureDengerousV.GetAddressOf());
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <returns>終了状態</returns>
bool GameEnemyManager::Update(DX::StepTimer const& timer, Player* player)
{
	m_respawnTime++;

	// カメラの更新
	mp_gameCamera->Update(timer, player);

	if (player->GetAssaultPoint() == 0)
	{
		m_assault = false;
	}
	else
	{
		m_assault = true;
	}

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
				if (player->GetAssaultPoint() == 0)
				{
					mp_enemy[i]->SetState(false);
					break;
				}

				// カウントしなおし
				m_respawnTime = 0;

				// 場所設定
				switch (player->GetAssaultPoint())
				{
				case 1: mp_enemy[i]->SetPos(Vector3(-35.0f, 5.0f, -25.5f + float(rand() % 20 - 10))); break;
				case 2: mp_enemy[i]->SetPos(Vector3(0.0f, 15.0f, 25.5f + float(rand() % 20 - 10))); break;
				case 3: mp_enemy[i]->SetPos(Vector3(0.0f + float(rand() % 20 - 10), 3.0f, 30.0f)); break;
				case 4: mp_enemy[i]->SetPos(Vector3(-35.0f + float(rand() % 20 - 10), 5.0f, -25.5f + float(rand() % 10 - 5))); break;
				case 5: mp_enemy[i]->SetPos(Vector3(22.5f + float(rand() % 40 - 20), 7.0f, -42.5f)); break;
				case 6: mp_enemy[i]->SetPos(Vector3(12.5f + float(rand() % 60 - 30), 7.0f, float(rand() & 10))); break;
				case 7: mp_enemy[i]->SetPos(Vector3(32.5f + float(rand() % 60 - 30), 7.0f, 7.5f + float(rand() % 30 - 15))); break;
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
		if (mp_enemy[i]->GetState() && !player->GetEnemyTime())
		{
			mp_enemy[i]->SetState(false);
			break;
		}
		// 敵の動き
		if (mp_enemy[i]->GetState())
		{
			mp_enemy[i]->SetVel(Vector3(float(player->GetPos().x - mp_enemy[i]->GetPos().x) / 40.0f, 
										float(player->GetPos().y - mp_enemy[i]->GetPos().y) / 40.0f, 
										float(player->GetPos().z - mp_enemy[i]->GetPos().z) / 40.0f));
			mp_enemy[i]->Update(timer);

			double baseLength = 150.0;
			m_compereLength[i] = (mp_enemy[i]->GetPos().x - player->GetPos().x)*(mp_enemy[i]->GetPos().x - player->GetPos().x) +
								 (mp_enemy[i]->GetPos().y - player->GetPos().y)*(mp_enemy[i]->GetPos().y - player->GetPos().y) +
								 (mp_enemy[i]->GetPos().z - player->GetPos().z)*(mp_enemy[i]->GetPos().z - player->GetPos().z);
			if (baseLength*baseLength > m_compereLength[i] * m_compereLength[i])
			{
				if (m_compereLength[i] <= m_compereLength[m_lengthTmp])
				{
					m_lengthTmp = i;
				}

				// プレイヤーとの位置の差分
				SimpleMath::Vector3 pDir = mp_gameCamera->GetCameraAngle();
				pDir.Normalize();
				SimpleMath::Vector3 playerDir = SimpleMath::Vector3(pDir);
				// 敵の向き
				SimpleMath::Vector3 eDir = mp_enemy[m_lengthTmp]->GetVel();
				eDir.Normalize();
				SimpleMath::Vector3 enemyPos = SimpleMath::Vector3(eDir);

				// ベクトルの長さを求める
				double lengthA = pow((playerDir.x * playerDir.x) + (playerDir.z * playerDir.z), 0.5);
				double lengthB = pow((enemyPos.x * enemyPos.x) + (enemyPos.z * enemyPos.z), 0.5);
				// 内積とベクトルの長さを使ってcosθを求める
				double cos_sita = playerDir.x * enemyPos.x + playerDir.z * enemyPos.z / (lengthA * lengthB);

				// cosθからθを求める
				double sita = acos(cos_sita);
				// デグリーで求める
				sita = sita * 180.0 / double(XM_PI);

				// 正の値だったらプレイヤーに対して左側、逆だったら右側
				float dirRL = -playerDir.z * enemyPos.x + playerDir.x * enemyPos.z;
				// 正の値だったらプレイヤーに対して後ろ側、逆だったら前側
				float dirFB = playerDir.x * enemyPos.x + playerDir.z * enemyPos.z;

				if (dirRL < 0.0f)m_dengerousDirLR = DIRECTION::RIGHT;
				else m_dengerousDirLR = DIRECTION::LEFT;
				if (dirRL > -0.3f && dirRL < 0.3f)
				{
					m_assault = false; 
					m_dengerousDirLR = DIRECTION::NONE;
				}
				if (dirFB < 0.0f)m_dengerousDirFB = DIRECTION::FRONT;
				else m_dengerousDirFB = DIRECTION::BACK;
			}
			else
			{
				m_dengerousDirFB = DIRECTION::NONE;
				m_dengerousDirLR = DIRECTION::NONE;
			}

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

	// 敵がいたら危険サイン表示
	if (m_assault == true)
	{
		/*if (m_dengerousDirFB == DIRECTION::FRONT)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousH.Get(), SimpleMath::Vector2(150.0f, 50.0f));
		}
		else if (m_dengerousDirFB == DIRECTION::BACK)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousH.Get(), SimpleMath::Vector2(150.0f, 750.0f));
		}*/
		if (m_dengerousDirLR == DIRECTION::RIGHT)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousV.Get(), SimpleMath::Vector2(700.0f, 50.0f));
		}
		else if (m_dengerousDirLR == DIRECTION::LEFT)
		{
			DrawManager::SingletonGetInstance().Draw(m_textureDengerousV.Get(), SimpleMath::Vector2(50.0f, 50.0f));
		}
	}
}

/// <summary>
/// 廃棄
/// </summary>
void GameEnemyManager::Depose()
{
}
