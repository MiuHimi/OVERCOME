//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.cpp
// Summary. GameDecorateObjectClass
// Date.    2019/07/28
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "GameDecorateObject.h"

#include "../../ExclusiveGameObject/ADX2Le.h"

// usingディレクトリ
using namespace DirectX;

// staticディレクトリ
const int GameDecorateObject::SHAKE_COUNT = 5;
const SimpleMath::Vector3 GameDecorateObject::SHAKE_VEL = SimpleMath::Vector3(0.2f, 0.0f, 0.0f);


/// <summary>
/// コンストラクタ
/// </summary>
GameDecorateObject::GameDecorateObject()
	: m_world(SimpleMath::Matrix::Identity),
	  m_doorRota{ SimpleMath::Matrix::Identity }, m_doorTrans{ SimpleMath::Vector3::Zero }, m_isMoveDoor{ false }, m_isShowDoor{ false },
	  m_chestRota{ SimpleMath::Matrix::Identity }, m_chestTrans{ SimpleMath::Vector3::Zero }, m_chestTempPos{ SimpleMath::Vector3::Zero }, 
	  m_shakeCount{ 0 }, m_shakeVelCount{ 0 }, m_shakeNeedCount{ 0 }, m_isChestOpen{ false },
	  mp_modelEnemyDoor {nullptr}, mp_modelSmallRoom(nullptr),
	  mp_modelEnemyChestOpen{nullptr}, mp_modelEnemyChestClose{ nullptr }
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameDecorateObject::~GameDecorateObject()
{
}

/// <summary>
/// 生成
/// </summary>
void GameDecorateObject::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");

	for (int i = 0; i < MAX_DOOR_NUM; i++)
	{
		// 敵出現ドアモデルの読み込み
		mp_modelEnemyDoor[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\wood_door.cmo", fx);
		// モデルにフォグをかける
		mp_modelEnemyDoor[i]->UpdateEffects([&](IEffect* effect)
		{
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(4.0f);
				fog->SetFogEnd(10.0f);
				fog->SetFogColor(Colors::Black);
			}
		});
	}

	// 部屋モデルの読み込み
	mp_modelSmallRoom = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\small_room.cmo", fx);
	// モデルにフォグをかける
	mp_modelSmallRoom->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(15.0f);
			fog->SetFogEnd(20.0f);
			fog->SetFogColor(Colors::Black);
		}
	});

	for (int i = 0; i < 3; i++)
	{
		// チェストモデルの読み込み
		mp_modelEnemyChestOpen[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost_chest_open.cmo", fx);
		mp_modelEnemyChestClose[i] = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\ghost_chest_close.cmo", fx);

		// モデルにフォグをかける
		mp_modelEnemyChestOpen[i]->UpdateEffects([&](IEffect* effect)
		{
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(15.0f);
				fog->SetFogEnd(20.0f);
				fog->SetFogColor(Colors::Black);
			}
		});
		// モデルにフォグをかける
		mp_modelEnemyChestClose[i]->UpdateEffects([&](IEffect* effect)
		{
			auto fog = dynamic_cast<IEffectFog*>(effect);
			if (fog)
			{
				fog->SetFogEnabled(true);
				fog->SetFogStart(15.0f);
				fog->SetFogEnd(20.0f);
				fog->SetFogColor(Colors::Black);
			}
		});
	}
}
/// <summary>
/// 初期化
/// </summary>
void GameDecorateObject::Initialize()
{
	for (int i = 0; i < MAX_DOOR_NUM; i++)
	{
		m_doorTrans[0] = SimpleMath::Vector3(25.0f, 0.0f, -27.0f);
		m_doorTrans[1] = SimpleMath::Vector3(0.0f, 0.0f, -28.0f);
		m_doorTrans[2] = SimpleMath::Vector3(-3.0f, 0.0f, -23.0f);
		m_doorTrans[3] = SimpleMath::Vector3(7.0f, 0.0f, 25.0f);
		m_doorTrans[4] = SimpleMath::Vector3(-13.0f, 0.0f, 23.0f);
		m_doorTrans[5] = SimpleMath::Vector3(-14.0f, 0.0f, 23.0f);

		m_isShowDoor[i] = true;
	}

	for (int i = 0; i < 3; i++)
	{
		m_chestTrans[0] = SimpleMath::Vector3(-1.5f, 2.0f, -37.5f);
		m_chestTrans[1] = SimpleMath::Vector3(-16.0f, 2.0f, -27.5f);
		m_chestTrans[2] = SimpleMath::Vector3(-9.0f, 2.0f, -37.5f);
		m_chestRota[0] = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(0.0f));
		m_chestRota[1] = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f));
		m_chestRota[2] = SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(45.0f));
		m_chestTempPos[0] = m_chestTrans[0];
		m_chestTempPos[1] = m_chestTrans[1];
		m_chestTempPos[2] = m_chestTrans[2];

		m_shakeNeedCount[0] = 100;
		m_shakeNeedCount[1] = 140;
		m_shakeNeedCount[2] = 190;
	}
}

/// <summary>
/// 更新
/// </summary>
/// <returns>終了状態</returns>
bool GameDecorateObject::Update(int roadID)
{
	// 敵出現ドアの更新
	UpdateEnemyDoor(roadID);

	// 敵出現チェストの更新
	UpdateEnemyChest(roadID);

	/*if (roadID != 32) return false;*/

	// チェストの更新
	/*for (int i = 0; i < 3; i++)
	{
		// 振動是非カウント
		m_shakeCount[i]++;

		if (m_shakeCount[i] / m_shakeNeedCount[i] >= 1)
		{
			// 振動方向用カウント
			m_shakeVelCount[i]++;

			// 変数初期化
			SimpleMath::Quaternion rotation =
				SimpleMath::Quaternion::Identity;
			SimpleMath::Vector3 vect = SimpleMath::Vector3::Zero;

			switch (i)
			{
			case 0:
				// 振動するために必要な角度の計算
				rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), 0.0f);
				// 振動する方向を算出
				vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
				break;
			case 1:
				// 振動するために必要な角度の計算
				rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(90.0f));
				// 振動する方向を算出
				vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
				break;
			case 2:
				// 振動するために必要な角度の計算
				rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(45.0f));
				// 振動する方向を算出
				vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
				break;
			}

			if (m_shakeVelCount[i] / SHAKE_COUNT == 0 || m_shakeVelCount[i] / SHAKE_COUNT == 2)
			{
				// 向かって右側に振動
				m_chestTrans[i] = m_chestTempPos[i] + vect;
			}
			else if (m_shakeVelCount[i] / SHAKE_COUNT == 1 || m_shakeVelCount[i] / SHAKE_COUNT == 3)
			{
				// 向かって左側に振動
				m_chestTrans[i] = m_chestTempPos[i] + -vect;
			}
			else if (m_shakeVelCount[i] / SHAKE_COUNT >= 4)
			{
				if (m_shakeVelCount[i] / SHAKE_COUNT >= 8)
				{
					// 扉オープン
					m_isChestOpen[i] = true;
				}
				if (m_shakeVelCount[i] / SHAKE_COUNT >= 12)
				{
					// カウントリセット
					m_shakeCount[i] = 0;
					m_shakeVelCount[i] = 0;
					// 座標を元の位置へ
					m_chestTrans[i] = m_chestTempPos[i];
					// 次開くまでの時間を計算
					m_shakeNeedCount[i] = 60 + (int)(rand()*(100 - 60 + 1.0) / (1.0 + RAND_MAX));

					// 扉クローズ
					m_isChestOpen[i] = false;
				}
			}
		}
	}*/

	return true;
}

/// <summary>
/// 描画
/// </summary>
void GameDecorateObject::Render(MatrixManager * matrixManager)
{
	// ドアの描画
	for (int i = 0; i < MAX_DOOR_NUM; i++)
	{
		switch (i)
		{
		case 0:
			m_world = SetWorldMatrix(m_world, 
				SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(m_doorTrans[0]));
			break;
		case 1:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(m_doorTrans[1]));
			break;
		case 2:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(90.0f)), SimpleMath::Matrix::CreateTranslation(m_doorTrans[2]));
			break;
		case 3:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(-90.0f)), SimpleMath::Matrix::CreateTranslation(m_doorTrans[3]));
			break;
		case 4:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(m_doorTrans[4]));
			break;
		case 5:
			m_world = SetWorldMatrix(m_world,
				SimpleMath::Matrix::CreateRotationY(XMConvertToRadians(180.0f)), SimpleMath::Matrix::CreateTranslation(m_doorTrans[5]));
			break;
		default:
			break;
		}

		// 表示できる状態なら
		if (m_isShowDoor[i])
		{
			// ドアの描画
			mp_modelEnemyDoor[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
				m_world, matrixManager->GetView(), matrixManager->GetProjection());
		}
	}

	// 小部屋の描画
	m_world = SetWorldMatrix(m_world,
		SimpleMath::Matrix::CreateRotationY(0.0f), SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-8.5f, 0.0f, -32.0f)));
	mp_modelSmallRoom->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		m_world, matrixManager->GetView(), matrixManager->GetProjection());

	// チェストの描画
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			m_world = SetWorldMatrix(m_world,
				m_chestRota[0], SimpleMath::Matrix::CreateTranslation(m_chestTrans[0]));
			break;
		case 1:
			m_world = SetWorldMatrix(m_world,
				m_chestRota[1], SimpleMath::Matrix::CreateTranslation(m_chestTrans[1]));
			break;
		case 2:
			m_world = SetWorldMatrix(m_world,
				m_chestRota[2], SimpleMath::Matrix::CreateTranslation(m_chestTrans[2]));
			break;
		default:
			break;
		}

		// チェストの描画
		if(m_isChestOpen[i])
		mp_modelEnemyChestOpen[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
			m_world, matrixManager->GetView(), matrixManager->GetProjection());
		else
			mp_modelEnemyChestClose[i]->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
				m_world, matrixManager->GetView(), matrixManager->GetProjection());
	}
}

/// <summary>
/// 廃棄処理
/// </summary>
void GameDecorateObject::Depose()
{
}

/// <summary>
/// 描画前にするワールド行列の設定
/// </summary>
/// <param name="rota">回転行列</param>
/// <param name="trans">移動行列</param>
SimpleMath::Matrix GameDecorateObject::SetWorldMatrix(SimpleMath::Matrix& world, SimpleMath::Matrix rota, SimpleMath::Matrix trans)
{
	world = SimpleMath::Matrix::Identity;
	world *= rota;
	world *= trans;

	return world;
}

/// <summary>
/// 敵出現ドアの更新
/// </summary>
/// <param name="roadID">道ID</param>
void GameDecorateObject::UpdateEnemyDoor(int roadID)
{
	if (roadID == 31 && m_doorTrans[0].x < 35.0f)
	{
		// SEがなってなかったら
		if (!m_isMoveDoor[0])
		{
			// ドアが開くSE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[0] = true;
		}
		// ドアを動かす
		m_doorTrans[0].x += 0.1f;
	}
	else
		m_isShowDoor[0] = true;
	if (roadID == 32 && m_doorTrans[1].x < 5.0f)
	{
		// SEがなってなかったら
		if (!m_isMoveDoor[1])
		{
			// ドアが開くSE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[1] = true;
		}
		// ドアを動かす
		m_doorTrans[1].x += 0.1f;
	}
	else
		m_isShowDoor[1] = true;
	if (roadID == 32 && m_doorTrans[2].z < -15.0f)
	{
		// SEがなってなかったら
		if (!m_isMoveDoor[2])
		{
			// ドアが開くSE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[2] = true;
		}
		// ドアを動かす
		m_doorTrans[2].z += 0.1f;
	}
	else
		m_isShowDoor[2] = true;
	if (roadID == 33 && m_doorTrans[3].z < 35.0f)
	{
		// SEがなってなかったら
		if (!m_isMoveDoor[3])
		{
			// ドアが開くSE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[3] = true;
		}
		// ドアを動かす
		m_doorTrans[3].z += 0.1f;
	}
	else
		m_isShowDoor[3] = true;
	if (roadID == 34 && m_doorTrans[4].x < -6.0f)
	{
		// SEがなってなかったら
		if (!m_isMoveDoor[4])
		{
			// ドアが開くSE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[4] = true;
		}
		// ドアを動かす
		m_doorTrans[4].x += 0.1f;
	}
	else
		m_isShowDoor[4] = true;
	if (roadID == 34 && m_doorTrans[5].x > -25.0f)
	{
		// SEがなってなかったら
		if (!m_isMoveDoor[5])
		{
			// ドアが開くSE
			ADX2Le* adx2le = ADX2Le::GetInstance();
			adx2le->Play(6);
			m_isMoveDoor[5] = true;
		}
		// ドアを動かす
		m_doorTrans[5].x -= 0.1f;
		m_isShowDoor[5] = true;
	}
	else if (roadID != 34 && m_doorTrans[5].x > -25.0f)
	{
		m_isShowDoor[5] = true;
	}
	else
		m_isShowDoor[5] = false;
}

/// <summary>
/// 敵出現チェストの更新
/// </summary>
/// <param name="roadID">道ID</param>
void GameDecorateObject::UpdateEnemyChest(int roadID)
{
	if (roadID != 32) return;

	// チェストの更新
	for (int i = 0; i < 3; i++)
	{
		// 振動是非カウント
		m_shakeCount[i]++;

		// まだ振動を開始してはいけないカウントだったら
		if (m_shakeCount[i] / m_shakeNeedCount[i] < 1)
		{
			continue;
		}

		// 振動方向用カウント
		m_shakeVelCount[i]++;

		// 変数初期化
		SimpleMath::Quaternion rotation = SimpleMath::Quaternion::Identity;
		SimpleMath::Vector3 vect = SimpleMath::Vector3::Zero;

		// 振動用の値の計算
		switch (i)
		{
		case 0:
			// 振動するために必要な角度の計算
			rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), 0.0f);
			// 振動する方向を算出
			vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
			break;
		case 1:
			// 振動するために必要な角度の計算
			rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(90.0f));
			// 振動する方向を算出
			vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
			break;
		case 2:
			// 振動するために必要な角度の計算
			rotation = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 0.1f, 0.0f), XMConvertToRadians(45.0f));
			// 振動する方向を算出
			vect = SimpleMath::Vector3::Transform(SHAKE_VEL, rotation);
			break;
		}

		// 振動関係の処理
		if (m_shakeVelCount[i] / SHAKE_COUNT == 0 || m_shakeVelCount[i] / SHAKE_COUNT == 2)
		{
			// 向かって右側に振動
			m_chestTrans[i] = m_chestTempPos[i] + vect;
		}
		else if (m_shakeVelCount[i] / SHAKE_COUNT == 1 || m_shakeVelCount[i] / SHAKE_COUNT == 3)
		{
			// 向かって左側に振動
			m_chestTrans[i] = m_chestTempPos[i] + -vect;
		}
		else
		{
			// 座標を元の位置へ
			m_chestTrans[i] = m_chestTempPos[i];
		}

		// 扉開閉関係の処理
		if (m_shakeVelCount[i] / SHAKE_COUNT >= 8 && m_shakeVelCount[i] / SHAKE_COUNT < 12)
		{
			// 扉オープン
			m_isChestOpen[i] = true;
		}
		else if (m_shakeVelCount[i] / SHAKE_COUNT >= 12)
		{
			// カウントリセット
			m_shakeCount[i] = 0;
			m_shakeVelCount[i] = 0;
			// 次開くまでの時間を計算
			m_shakeNeedCount[i] = 60 + rand()*(90 - 60 + 1) / (1 + RAND_MAX);

			// 扉クローズ
			m_isChestOpen[i] = false;
		}
	}
}
