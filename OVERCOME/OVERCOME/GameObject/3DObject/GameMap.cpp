//////////////////////////////////////////////////////////////
// File.    GameRoad.cpp
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <fstream>
#include <sstream>

#include "../../pch.h"
#include "../SceneObject/SceneManager.h"
#include "GameMap.h"
#include "../3DObject/Player.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

int SceneManager::m_stageID;

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

	// ファイルパス
	std::wstring filePath;
	// ステージ番号
	int fileNumber = SceneManager::GetStageNum();

	// 文字列を連結しモデルのパスを生成
	std::wstringstream ssModel;
	ssModel << "Resources\\Models\\map0" << fileNumber << ".cmo";
	filePath = ssModel.str();
	// モデルをロードしてモデルハンドルを取得する
	m_modelMap = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), filePath.c_str(), fx);

	// 文字列を連結し衝突判定モデルのパスを生成
	std::wstringstream ssCollision;
	ssCollision << "Resources\\StageMap\\stage0" << fileNumber << ".obj";
	filePath = ssCollision.str();
	// メッシュ衝突判定
	m_collisionStage = std::make_unique<CollisionMesh>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), filePath.c_str());

}

/// <summary>
/// 更新
/// </summary>
bool GameMap::Update(DX::StepTimer const & timer, Player *player)
{
	int id;
	SimpleMath::Vector3 s;
	SimpleMath::Vector3 playerPos = player->GetPos();
	SimpleMath::Vector3 v[2] = { SimpleMath::Vector3(playerPos.x, 100.0f, playerPos.z), SimpleMath::Vector3(playerPos.x, -100.0f, playerPos.z) };
	// 道とプレイヤーの当たり判定を行う
	if (m_collisionStage->HitCheck_Segment(v[0], v[1], &id, &s) == true)
	{
		// プレイヤーの位置を設定する
		player->SetHeightPos(s.y);
	}

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
