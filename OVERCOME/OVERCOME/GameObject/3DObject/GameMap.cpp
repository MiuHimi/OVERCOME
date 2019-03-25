//////////////////////////////////////////////////////////////
// File.    GameRoad.cpp
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "../SceneObject/SceneManager.h"
#include "GameMap.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
GameMap::GameMap()
	: m_modelMap(nullptr)
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameMap::~GameMap()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameMap::Initialize()
{
}
/// <summary>
/// 生成
/// </summary>
void GameMap::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelMap = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\wood_map01.cmo", fx);
}

/// <summary>
/// 更新
/// </summary>
bool GameMap::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// 描画
/// </summary>
void GameMap::Render(MatrixManager * matrixManager)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	// 描画
	m_modelMap->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// 破棄
/// </summary>
void GameMap::Depose()
{
}
