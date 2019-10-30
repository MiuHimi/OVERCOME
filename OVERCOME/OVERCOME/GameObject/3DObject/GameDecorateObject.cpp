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


/// <summary>
/// コンストラクタ
/// </summary>
GameDecorateObject::GameDecorateObject()
	: m_world(SimpleMath::Matrix::Identity),
	  m_doorRota{ SimpleMath::Matrix::Identity }, m_doorTrans{ SimpleMath::Vector3::Zero }, m_isMoveDoor{ false }, m_isShowDoor{ false },
	  mp_modelEnemyDoor {nullptr}, mp_modelSmallRoom(nullptr)
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

	mp_modelSmallRoom = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\small_room.cmo", fx);

	// モデルにフォグをかける
	mp_modelSmallRoom->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(8.0f);
			fog->SetFogEnd(15.0f);
			fog->SetFogColor(Colors::Black);
		}
	});
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
}

/// <summary>
/// 更新
/// </summary>
/// <returns>終了状態</returns>
bool GameDecorateObject::Update(int roadID)
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
	else if(roadID != 34 && m_doorTrans[5].x > -25.0f)
	{
		m_isShowDoor[5] = true;
	}
	else
		m_isShowDoor[5] = false;

	return false;
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
