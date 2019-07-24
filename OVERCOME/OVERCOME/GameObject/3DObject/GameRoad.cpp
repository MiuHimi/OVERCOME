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

#include "../../pch.h"
#include "../SceneObject/SceneManager.h"
#include "GameRoad.h"

#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;

//int SceneManager::m_stageID;


/// <summary>
/// コンストラクタ
/// </summary>
GameRoad::GameRoad()
{
}
/// <summary>
/// デストラクタ
/// </summary>
GameRoad::~GameRoad()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void GameRoad::Initialize()
{
	// ステージマップの読み込み
	std::string filePath = "Resources\\StageMap\\stage";
	std::ostringstream os;
	int stageNum = 1;
	os << stageNum;
	filePath += os.str() + ".csv";

	// ステージマップの取得
	std::ifstream ifs(filePath);
	std::string line;
	if (!ifs)
	{
		// ファイル読み込み失敗
		throw std::range_error("Read failure.");
	}

	// 初期値とゴールの位置の記憶
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		int i = 0;
		while (getline(stream, buf, ','))
		{
			int pos = std::atoi(buf.c_str());
			m_posStartEnd[i].x = float(pos / 100);
			m_posStartEnd[i].y = float(pos % 100);
			i++;

			if (i > 1) break;
		}
		if (i > 1) break;
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
			// 二桁のうち、十の桁を道の種類(襲撃ポイントやスタートゴールなど)、一の桁を襲撃するタイミング順に設定
			m_road[j][i].roadType = roadType / 10;
			m_road[j][i].roadNum = roadType % 10;
			i++;
		}
		j++;
	}

	
	// 道の座標設定
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			// X座標設定
			int x = i;
			if (i >= 0 && i < 10)
			{
				// 座標が[x < 0]の場合
				m_road[j][i].pos.x = float((m_maxFloorWidth / 2) - m_roadBlockSize * x - m_roadBlockSize / 2);
				m_road[j][i].pos.x *= -1.0f;
			}
			else if (i >= 10)
			{
				// 座標が[x > 0]の場合
				x -= 10;
				m_road[j][i].pos.x = float(m_roadBlockSize * x + m_roadBlockSize / 2);
			}
			// Y座標設定
			m_road[j][i].pos.y = 0.0f;
			// Z座標設定
			int z = j;
			if (j >= 0 && j < 10)
			{
				// 座標が[z < 0]の場合
				m_road[j][i].pos.z = float((m_maxFloorHeight / 2) - m_roadBlockSize * z - m_roadBlockSize / 2);
				m_road[j][i].pos.z *= -1.0f;
			}
			else if (j >= 10)
			{
				// 座標が[z > 0]の場合
				z -= 10;
				m_road[j][i].pos.z = float(m_roadBlockSize * z + m_roadBlockSize / 2);
			}
		}
	}
	
	
}
/// <summary>
/// 生成処理
/// </summary>
void GameRoad::Create()
{
	// フォグの設定
	SetFogEffectDistance(15.0f, 20.0f);
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
void GameRoad::Render(MatrixManager* matrixManager)
{
}

/// <summary>
/// 削除処理
/// </summary>
void GameRoad::Depose()
{
}

/// <summary>
/// フォグのスタートとエンドを設定
/// </summary>
/// <param name="start">効果がかかり始める距離</param>
/// <param name="end">効果が完全にかかる距離</param>
void GameRoad::SetFogEffectDistance(float start, float end)
{
	// フォグの設定
	//m_modelRoadStraight->UpdateEffects([&](IEffect* effect)
	//{
	//	auto fog = dynamic_cast<IEffectFog*>(effect);
	//	if (fog)
	//	{
	//		fog->SetFogEnabled(true);
	//		fog->SetFogStart(start); // assuming RH coordiantes
	//		fog->SetFogEnd(end);
	//		fog->SetFogColor(Colors::Black);
	//	}
	//});
}
