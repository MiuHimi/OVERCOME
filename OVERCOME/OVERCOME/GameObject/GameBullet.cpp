//////////////////////////////////////////////////////////////
// File.    GameBullet.cpp
// Summary. GameBulletClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"
#include "GameBullet.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameBullet::GameBullet(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model)
	: m_pos(0.0f, 0.0f, 0.0f),
	  m_vel(0.0f, 0.0f, 0.0f),
	  m_state(false),
	  m_world(DirectX::SimpleMath::Matrix::Identity),
	  mp_modelBullet(nullptr)
{
	m_pos = pos;
	m_vel = vel;

	m_state = stateFlag;
}
/// <summary>
/// デストラクタ
/// </summary>
GameBullet::~GameBullet()
{
}

void GameBullet::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");

	// モデルを作成
	mp_modelBullet = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\bullet.cmo", fx);
	// 衝突判定用モデル設定
	Obj3D::SetModel(mp_modelBullet.get());

	m_sphere.c = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_sphere.r = 0.0f;
	SetCollision(m_sphere);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <returns>終了状態</returns>
bool GameBullet::Update(DX::StepTimer const & timer)
{
	m_pos += m_vel;

	// ワールド行列の作成
	m_world = SimpleMath::Matrix::CreateTranslation(m_pos);

	m_sphere.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);      // 境界球の中心
	m_sphere.r = 1.0f;                                                         // 半径
	SetCollision(m_sphere);

	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameBullet::Render()
{
	mp_modelBullet->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		                 m_world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());
}
