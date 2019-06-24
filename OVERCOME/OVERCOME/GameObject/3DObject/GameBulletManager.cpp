//////////////////////////////////////////////////////////////
// File.    GameBulletManager.cpp
// Summary. GameBulletManagerClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "GameBulletManager.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"
#include "../../Utility/InputManager.h"

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// コンストラクタ
/// </summary>
GameBulletManager::GameBulletManager()
	: m_reloadTime(0),
	 m_shootingFlag(false)
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameBulletManager::~GameBulletManager()
{
	for (int i = 0; i < m_maxBulletNum; i++)
	{
		delete mp_bullet[i];
		mp_bullet[i] = nullptr;
	}
}

/// <summary>
/// 初期化
/// </summary>
void GameBulletManager::Initialize()
{
	// メモリーリーク
	/*for (int i = 0; i < m_maxBulletNum; i++)
	{
		GameBullet* bullet = new GameBullet(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		// 弾の情報を格納
		mp_bullet.push_back(bullet);
	}*/

	//mp_bullet.resize(m_maxBulletNum);
	// ここまで
}
/// <summary>
/// 生成
/// </summary>
void GameBulletManager::Create()
{
	/*for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		GameBullet* bullet = new GameBullet(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		// 弾の情報を格納
		mp_bullet.push_back(bullet);
	}*/

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		mp_bullet[i] = new GameBullet(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
	}

	//for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	//{
	//	/*std::unique_ptr<Model> model;
	//	model = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
	//	(*it)->SetModel(model.get());
	//	SetModel(model.get());*/

	//	/*// モデルを作成
	//	std::unique_ptr<Model> model;
	//	model = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
	//	// モデルをそれぞれ(要素)にセット
	//	(*it)->SetModel(std::move(model));
	//	// 衝突判定用モデル設定
	//	SetModel(model.get());*/

	//	(*it)->Create();
	//}

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		mp_bullet[i]->Create();
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <returns>終了状態</returns>
bool GameBulletManager::Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3 ShootPos, DirectX::SimpleMath::Vector3 cameraDir)
{
	InputManager::SingletonGetInstance().GetTracker().Update(InputManager::SingletonGetInstance().GetMouseState());

	// 撃った直後からカウント開始
	if (m_shootingFlag)
	{
		m_reloadTime++;
	}

	// 一秒経ったら再び射撃可能に
	if (m_reloadTime >= m_needReloadTime)
	{
		m_reloadTime = 0;
		m_shootingFlag = false;
	}

	//for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	//{
	//	if (!(*it)->GetState())
	//	{
	//		if (!m_shootingFlag)
	//		{
	//			if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
	//			//if(InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	//			{
	//				// 残弾があったら発射準備
	//				(*it)->SetState(true);
	//				(*it)->SetPos(ShootPos);
	//				(*it)->SetVel(cameraDir);
	//				m_shootingFlag = true;
	//				break;
	//			}
	//		}
	//	}
	//}

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		if (!mp_bullet[i]->GetState())
		{
			if (!m_shootingFlag)
			{				
				if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
				{
					// 残弾があったら発射準備
					mp_bullet[i]->SetState(true);
					mp_bullet[i]->SetPos(ShootPos);
					SimpleMath::Vector3 vel = cameraDir;
					vel /= 2.0f;
					mp_bullet[i]->SetVel(vel);
					m_shootingFlag = true;
					break;
				}
			}
		}
	}

	//for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	//{
	//	// 発射できる弾の更新
	//	if ((*it)->GetState())
	//	{
	//		(*it)->Update(timer);
	//	}
	//}

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		if (mp_bullet[i]->GetState())
		{
			mp_bullet[i]->Update(timer);
		}
	}

	// 衝突判定情報の設定
	/*for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		(*it)->SetCollision((*it)->GetCollide());
	}*/

	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameBulletManager::Render(MatrixManager* matrixManager)
{
	/*for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		if ((*it)->GetState())
		{
			(*it)->Render();
		}
	}*/

	for (int i = 0; i < m_maxBulletNum; i++)
	{
		if (mp_bullet[i]->GetState())
		{
			mp_bullet[i]->Render(matrixManager);
		}
	}
}

/// <summary>
/// 廃棄
/// </summary>
void GameBulletManager::Depose()
{
}
