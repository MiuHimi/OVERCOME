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

	// フォグの設定
	SetFogEffectDistance(10.0f, 30.0f);

	// 衝突判定用オブジェクト生成
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
void GameFloor::Render(MatrixManager* matrixManager)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	// 床の描画
	world = SimpleMath::Matrix::Identity;
	m_modelLattice->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		                 world, matrixManager->GetView(), matrixManager->GetProjection());
}

/// <summary>
/// 削除処理
/// </summary>
void GameFloor::Depose()
{
}

/// <summary>
/// フォグのスタートとエンドを設定
/// </summary>
/// <param name="start">効果がかかり始める距離</param>
/// <param name="end">効果が完全にかかる距離</param>
void GameFloor::SetFogEffectDistance(float start, float end)
{
	m_modelLattice->UpdateEffects([&](IEffect* effect)
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
