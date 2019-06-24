//////////////////////////////////////////////////////////////
// File.    GameEnemy.cpp
// Summary. GameEnemyClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../../pch.h"
#include "GameEnemy.h"

#include "../../Utility/DeviceResources.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameEnemy::GameEnemy(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 vel, bool stateFlag, DirectX::Model* model)
	: m_pos(0.0f, 0.0f, 0.0f),
	  m_vel(0.0f, 0.0f, 0.0f),
	  m_dir(0.0f, 0.0f, 0.0f),
	  m_rotaX(SimpleMath::Quaternion::Identity),
   	  m_rotaY(SimpleMath::Quaternion::Identity),
	  m_state(false),
	  m_world(DirectX::SimpleMath::Matrix::Identity),
	  mp_modelEnemy(nullptr)
{
	m_pos = pos;
	m_vel = vel;
	m_dir = SimpleMath::Vector3(0.0f, 0.0f, 1.0f);

	m_state = stateFlag;
}
/// <summary>
/// デストラクタ
/// </summary>
GameEnemy::~GameEnemy()
{
}

void GameEnemy::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルを作成
	mp_modelEnemy = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\enemy.cmo", fx);
	// 衝突判定用モデル設定
	Obj3D::SetModel(mp_modelEnemy.get());

	// フォグの設定
	SetFogEffectDistance(10.0f, 30.0f);

	// 衝突判定用オブジェクト設定
	m_sphere.c = SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_sphere.r = 1.0f;
	SetCollision(m_sphere);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="timer">経過時間</param>
/// <returns>終了状態</returns>
bool GameEnemy::Update(DX::StepTimer const & timer)
{
	// 移動
	m_pos += m_vel;

	// 回転行列を作成
	SimpleMath::Matrix rota = SimpleMath::Matrix::CreateFromQuaternion(m_rotaX) * SimpleMath::Matrix::CreateFromQuaternion(m_rotaY);
	// ワールド行列を作成
	m_world = rota * SimpleMath::Matrix::CreateTranslation(m_pos);

	// 衝突判定用オブジェクト更新
	m_sphere.c = DirectX::SimpleMath::Vector3(m_pos.x, m_pos.y, m_pos.z);      // 境界球の中心
	m_sphere.r = 1.0f;                                                         // 半径
	SetCollision(m_sphere);

	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameEnemy::Render(MatrixManager* matrixManager)
{
	mp_modelEnemy->Draw(DX::DeviceResources().SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		                m_world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// フォグのスタートとエンドを設定
/// </summary>
/// <param name="start">効果がかかり始める距離</param>
/// <param name="end">効果が完全にかかる距離</param>
void GameEnemy::SetFogEffectDistance(float start, float end)
{
	mp_modelEnemy->UpdateEffects([&](IEffect* effect)
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
