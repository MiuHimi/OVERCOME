//////////////////////////////////////////////////////////////
// File.    GameTarget.cpp
// Summary. GameTargetClass
// Date.    2018/07/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include <fstream>
#include <string>
#include <sstream>

#include "../pch.h"
#include "GameTarget.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

//int SceneManager::m_stageID;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
GameTarget::GameTarget() : m_stageCount(0)
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameTarget::~GameTarget()
{
	//GameRoad::Depose();
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			delete mp_targetCollideObject[j][i];
			mp_targetCollideObject[j][i] = nullptr;
		}
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void GameTarget::Initialize()
{
	// ステージマップの読み込み
	std::string filePath = "Resources\\TargetMap\\Stage";
	std::ostringstream os;
	m_stageCount = SceneManager::GetStageNum();
	if (m_stageCount != 1)m_stageCount = 1;
	os << m_stageCount;
	filePath += os.str() + ".csv";

	// ステージマップの取得
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
			int targetType = std::atoi(buf.c_str());
			// 二桁のうち、十の桁を道路の種類、一の桁を回転角に設定
			m_targetObject[j][i].height = targetType / 10;
			m_targetObject[j][i].rotaAngle = targetType % 10;
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
				m_targetObject[j][i].pos.x = float((m_maxFloorWidth / 2) - m_roadBlockSize * x - m_roadBlockSize / 2);
				m_targetObject[j][i].pos.x *= -1.0f;
			}
			else if (i >= 10)
			{
				// 座標が[x > 0]の場合
				x -= 10;
				m_targetObject[j][i].pos.x = float(m_roadBlockSize * x + m_roadBlockSize / 2);
			}
			// Y座標設定
			m_targetObject[j][i].pos.y = m_targetObject[j][i].height + m_targetRadius;
			// Z座標設定
			int z = j;
			if (j >= 0 && j < 10)
			{
				// 座標が[z < 0]の場合
				m_targetObject[j][i].pos.z = float((m_maxFloorHeight / 2) - m_roadBlockSize * z - m_roadBlockSize / 2);
				m_targetObject[j][i].pos.z *= -1.0f;
			}
			else if (j >= 10)
			{
				// 座標が[z > 0]の場合
				z -= 10;
				m_targetObject[j][i].pos.z = float(m_roadBlockSize * z + m_roadBlockSize / 2);
			}
		}
	}
}
/// <summary>
/// 生成処理
/// </summary>
void GameTarget::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelTarget = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\target.cmo", fx);
	
	Collision::Box box;
	box.c = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);      // 境界箱の中心
	box.r = DirectX::SimpleMath::Vector3(50.0f, 0.0f, 50.0f);    // 各半径

	// 道路の作成
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			mp_targetCollideObject[j][i] = new CollisionBox();
			if(m_targetObject[j][i].height != 0)     mp_targetCollideObject[j][i]->SetModel(m_modelTarget.get());
			else mp_targetCollideObject[j][i]->SetModel(NULL);
			
		}
	}

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			if (m_targetObject[j][i].height != 0)
			{
				box.c = DirectX::SimpleMath::Vector3(m_targetObject[j][i].pos.x, m_targetObject[j][i].pos.y, m_targetObject[j][i].pos.z);                              // 箱型境界の中心
				if (m_targetObject[j][i].rotaAngle == 0 || m_targetObject[j][i].rotaAngle == 2)box.r = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 0.1f);               // 各半径設定
				if (m_targetObject[j][i].rotaAngle == 1 || m_targetObject[j][i].rotaAngle == 1)box.r = DirectX::SimpleMath::Vector3(0.1f, 1.0f, 1.0f);               // 各半径設定

				mp_targetCollideObject[j][i]->SetCollision(box);
			}
		}
	}
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool GameTarget::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// 描画処理
/// </summary>
void GameTarget::Render()
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	//m_modelTarget->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
	//	                world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());

	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// 座標確定
			trans = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(m_targetObject[j][i].pos.x, m_targetObject[j][i].pos.y, m_targetObject[j][i].pos.z));

			// 回転設定
			float angle = float(m_targetObject[j][i].rotaAngle * 90.0f);  // 回転角を設定( (0 or 1 or 2 or 3) * 90.0f )
			// 回転確定
			rot = SimpleMath::Matrix::CreateFromQuaternion(SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(angle)));

			// 行列確定
			world = SimpleMath::Matrix::Identity;
			world *= rot * trans;

			auto& res = DX::DeviceResources::SingletonGetInstance();
			// 描画道路選択
			
			if (m_targetObject[j][i].height != 0)m_modelTarget->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());
			// デバッグ道路描画
			if(m_targetObject[j][i].height != 0)mp_targetCollideObject[j][i]->DrawDebugCollision();
		}
	}
}

/// <summary>
/// 削除処理
/// </summary>
void GameTarget::Depose()
{
}
