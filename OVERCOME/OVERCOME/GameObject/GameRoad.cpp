//////////////////////////////////////////////////////////////
// File.    GameRoad.cpp
// Summary. GameRoadClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <fstream>
#include <string>
#include <sstream>

#include "../pch.h"
#include "GameRoad.h"

#include "../ExclusiveGameObject/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

int SceneManager::m_stageNum;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
GameRoad::GameRoad(Game* game) : mp_game(game)
                                ,m_stageNum(0)
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameRoad::~GameRoad()
{
	//GameRoad::Depose();
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			delete mp_roadCollideObject[j][i];
			mp_roadCollideObject[j][i] = nullptr;
		}
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void GameRoad::Initialize()
{
	// ステージマップの読み込み
	std::string filePath = "Resources\\StageMap\\Stage";
	std::ostringstream os;
	m_stageNum = SceneManager::GetStageNum();
	os << m_stageNum;
	filePath += os.str() + ".csv";

	// ステージマップの取得
	//std::ifstream ifs(L"Resources\\StageMap\\Stage02.csv");
	std::ifstream ifs(filePath);
	std::string line;
	if (!ifs)
	{
		// ファイル読み込み失敗
		throw std::range_error("Read failure.");
	}

	int j = 0;
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		int i = 0;
		while (getline(stream, buf, ','))
		{
			// 文字を数値に変換
			int roadType = std::atoi(buf.c_str());
			// 二桁のうち、十の桁を道路の種類、一の桁を回転角に設定
			m_roadObject[j][i].roadType = roadType / 10;
			m_roadObject[j][i].rotaAngle = roadType % 10;
			i++;
		}
		j++;
	}

	// 道路の座標設定
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// X座標設定
			int x = i;
			if (i >= 0 && i < 10)
			{
				// 座標が[x < 0]の場合
				m_roadObject[j][i].pos.x = float((m_maxFloorWidth / 2) - m_roadBlockSize * x - m_roadBlockSize / 2);
				m_roadObject[j][i].pos.x *= -1.0f;
			}
			else if (i >= 10)
			{
				// 座標が[x > 0]の場合
				x -= 10;
				m_roadObject[j][i].pos.x = float(m_roadBlockSize * x + m_roadBlockSize / 2);
			}
			// Y座標設定
			m_roadObject[j][i].pos.y = 0.0f;
			// Z座標設定
			int z = j;
			if (j >= 0 && j < 10)
			{
				// 座標が[z < 0]の場合
				m_roadObject[j][i].pos.z = float((m_maxFloorHeight / 2) - m_roadBlockSize * z - m_roadBlockSize / 2);
				m_roadObject[j][i].pos.z *= -1.0f;
			}
			else if (j >= 10)
			{
				// 座標が[z > 0]の場合
				z -= 10;
				m_roadObject[j][i].pos.z = float(m_roadBlockSize * z + m_roadBlockSize / 2);
			}
		}
	}
}
/// <summary>
/// 生成処理
/// </summary>
void GameRoad::Create(Game* game)
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelRoadStraight = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_straight.cmo", fx);
	m_modelRoadStop = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_stop.cmo", fx);
	m_modelRoadCurve = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_curve.cmo", fx);
	m_modelRoadBranch = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_branch.cmo", fx);

	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);      // 境界箱の中心
	box.r = DirectX::SimpleMath::Vector3(50.0f, 0.0f, 50.0f);    // 各半径

	// 道路の作成
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			mp_roadCollideObject[j][i] = new CollisionBox();
			mp_roadCollideObject[j][i]->SetGame(game);
			if (m_roadObject[j][i].roadType == 1)     mp_roadCollideObject[j][i]->SetModel(m_modelRoadStraight.get());
			else if (m_roadObject[j][i].roadType == 2)mp_roadCollideObject[j][i]->SetModel(m_modelRoadStop.get());
			else if (m_roadObject[j][i].roadType == 3)mp_roadCollideObject[j][i]->SetModel(m_modelRoadCurve.get());
			else if (m_roadObject[j][i].roadType == 4)mp_roadCollideObject[j][i]->SetModel(m_modelRoadBranch.get());
			else(mp_roadCollideObject[j][i]->SetModel(NULL));
		}
	}

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			if (m_roadObject[j][i].roadType == 1)
			{
				box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);                              // 箱型境界の中心
				if (m_roadObject[j][i].rotaAngle == 0 || m_roadObject[j][i].rotaAngle == 2)box.r = DirectX::SimpleMath::Vector3(1.5f, 1.0f, 2.5f);               // 各半径設定
				if (m_roadObject[j][i].rotaAngle == 1 || m_roadObject[j][i].rotaAngle == 1)box.r = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 1.5f);               // 各半径設定
				//m_roadStraight->SetCollision(box);
				//m_roadStraight->DrawDebugCollision();
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
			if (m_roadObject[j][i].roadType == 2)
			{
				if (m_roadObject[j][i].rotaAngle == 0)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z + 0.5f);   // 箱型境界の中心
				if (m_roadObject[j][i].rotaAngle == 1)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x + 0.5f, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // 箱型境界の中心
				if (m_roadObject[j][i].rotaAngle == 2)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z - 0.5f);   // 箱型境界の中心
				if (m_roadObject[j][i].rotaAngle == 3)  box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x - 0.5f, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // 箱型境界の中心
				if (m_roadObject[j][i].rotaAngle == 0 || m_roadObject[j][i].rotaAngle == 2)box.r = DirectX::SimpleMath::Vector3(1.5f, 1.0f, 2.0f);               // 各半径設定
				if (m_roadObject[j][i].rotaAngle == 1 || m_roadObject[j][i].rotaAngle == 3)box.r = DirectX::SimpleMath::Vector3(2.0f, 1.0f, 1.5f);               // 各半径設定
				//m_roadStop->SetCollision(box);
				//m_roadStop->DrawDebugCollision();
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
			if (m_roadObject[j][i].roadType == 3)
			{
				box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // 箱型境界の中心
				box.r = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 2.5f);               // 各半径設定
				//m_roadCurve->SetCollision(box);
				//m_roadCurve->DrawDebugCollision();
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
			if (m_roadObject[j][i].roadType == 4)
			{
				box.c = DirectX::SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z);   // 箱型境界の中心
				box.r = DirectX::SimpleMath::Vector3(2.5f, 1.0f, 2.5f);               // 各半径設定
				mp_roadCollideObject[j][i]->SetCollision(box);
			}
		}
	}
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool GameRoad::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// 描画処理
/// </summary>
void GameRoad::Render(DirectX::SimpleMath::Matrix view)
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// 座標確定
			trans = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(m_roadObject[j][i].pos.x, m_roadObject[j][i].pos.y, m_roadObject[j][i].pos.z));

			// 回転設定
			float angle = float(m_roadObject[j][i].rotaAngle * 90.0f);  // 回転角を設定( (0 or 1 or 2 or 3) * 90.0f )
			// 回転確定
			rot = SimpleMath::Matrix::CreateFromQuaternion(SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(angle)));

			// 行列確定
			world = SimpleMath::Matrix::Identity;
			world *= rot * trans;

			auto& res = DX::DeviceResources::SingletonGetInstance();
			DirectX::SimpleMath::Matrix& projection = MatrixManager::GetProjectionMatrix();
			// 描画道路選択
			int roadType = m_roadObject[j][i].roadType;
			switch (roadType)
			{
			case 0: break;                                                                                                                  // 何もなし
			case 1: m_modelRoadStraight->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection); break;   // 直線道路
			case 2: m_modelRoadStop->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection);     break;   // 末端道路
			case 3: m_modelRoadCurve->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection);    break;   // 曲線道路
			case 4: m_modelRoadBranch->Draw(res.GetD3DDeviceContext(), *mp_game->GetState(), world, view, /*mp_game->GetProjection()*/projection);    break;   // 分岐道路
			}
			// デバッグ道路描画
			//if(m_roadObject[j][i].roadType == 1 || m_roadObject[j][i].roadType == 2 || m_roadObject[j][i].roadType == 3)mp_roadCollideObject[j][i]->DrawDebugCollision(view);
		}
	}
}

/// <summary>
/// 削除処理
/// </summary>
void GameRoad::Depose()
{
	// ゲームオブジェクトを削除
	delete mp_game;
	mp_game = NULL;
}
