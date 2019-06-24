//////////////////////////////////////////////////////////////
// File.    GameBullet.cpp
// Summary. GameBulletClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "GameBullet.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

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

	// フォグの設定
	SetFogEffectDistance(10.0f, 30.0f);

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
void GameBullet::Render(MatrixManager* matrixManager)
{
	mp_modelBullet->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		                 m_world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// フォグのスタートとエンドを設定
/// </summary>
/// <param name="start">効果がかかり始める距離</param>
/// <param name="end">効果が完全にかかる距離</param>
void GameBullet::SetFogEffectDistance(float start, float end)
{
	mp_modelBullet->UpdateEffects([&](IEffect* effect)
	{
		auto fog = dynamic_cast<IEffectFog*>(effect);
		if (fog)
		{
			fog->SetFogEnabled(true);
			fog->SetFogStart(start); // assuming RH coordiantes
			fog->SetFogEnd(end);
			fog->SetFogColor(Colors::Black);
		}
	});
}
