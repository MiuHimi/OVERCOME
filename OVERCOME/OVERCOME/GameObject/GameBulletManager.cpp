//////////////////////////////////////////////////////////////
// File.    GameBulletManager.cpp
// Summary. GameBulletManagerClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/InputManager.h"
#include "GameBulletManager.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

#include "../Utility/GameDebug.h"

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
}

/// <summary>
/// 初期化
/// </summary>
void GameBulletManager::Initialize()
{
	for (int i = 0; i < m_maxBulletNum; i++)
	{
		GameBullet* bullet = new GameBullet(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false, nullptr);
		// ブロックの情報を格納
		mp_bullet.push_back(bullet);
	}
}
/// <summary>
/// 生成
/// </summary>
void GameBulletManager::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");

	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it) 
	{
		/*std::unique_ptr<Model> model;
		model = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
		(*it)->SetModel(model.get());
		SetModel(model.get());*/

		// モデルを作成
		std::unique_ptr<Model> model;
		model = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
		// モデルをそれぞれ(要素)にセット
		(*it)->SetModel(std::move(model));
		// 衝突判定用モデル設定
		SetModel(model.get());
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <returns>終了状態</returns>
bool GameBulletManager::Update(DX::StepTimer const& timer, DirectX::SimpleMath::Vector3 ShootPos, DirectX::SimpleMath::Vector3 cameraDir)
{
	//InputManager::SingletonGetInstance().Update();

	if (m_shootingFlag)
	{
		m_reloadTime++;
	}

	if (m_reloadTime >= m_needReloadTime)
	{
		m_reloadTime = 0;
		m_shootingFlag = false;
	}

	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		if (!(*it)->GetState())
		{
			if (!m_shootingFlag)
			{
				if (InputManager::SingletonGetInstance().GetTracker().leftButton == Mouse::ButtonStateTracker::ButtonState::PRESSED)
				//if(InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
				{
					// 残弾があったら発射準備
					(*it)->SetState(true);
					(*it)->SetPos(ShootPos);
					(*it)->SetVel(cameraDir);
					m_shootingFlag = true;
					break;
				}
			}
		}
		
	}

	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		// 発射できる弾の更新
		if ((*it)->GetState())
		{
			(*it)->Update(timer);
		}
	}

	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameBulletManager::Render()
{
	int i = 0;
	for (auto it = std::begin(mp_bullet); it != std::end(mp_bullet); ++it)
	{
		if ((*it)->GetState())
		{
			(*it)->Render();
		}

		i++;
	}
}

/// <summary>
/// 廃棄
/// </summary>
void GameBulletManager::Depose()
{
}
