//////////////////////////////////////////////////////////////
// File.    GameFloor.cpp
// Summary. GameFloorClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "GameFloor.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
GameFloor::GameFloor()
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameFloor::~GameFloor()
{
	//GameFloor::Depose();
}

/// <summary>
/// 初期化処理
/// </summary>
void GameFloor::Initialize()
{
}
/// <summary>
/// 生成処理
/// </summary>
void GameFloor::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelLattice = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\floor01.cmo", fx);

	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);      // 境界箱の中心
	box.r = DirectX::SimpleMath::Vector3(50.0f, 0.0f, 50.0f);    // 各半径0.5

	// 床の作成
	SetModel(m_modelLattice.get());
	SetCollision(box);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool GameFloor::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// 描画処理
/// </summary>
void GameFloor::Render()
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	// 床の描画
	world = SimpleMath::Matrix::Identity;
	m_modelLattice->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		                 world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());
}

/// <summary>
/// 削除処理
/// </summary>
void GameFloor::Depose()
{
}
