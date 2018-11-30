//////////////////////////////////////////////////////////////
// File.    SceneStageSelect.cpp
// Summary. SceneStageSelectClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "SceneManager.h"
#include "SceneStageSelect.h"

#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"
#include "../../Utility/GameDebug.h"

// usingディレクトリ
using namespace DirectX;
//using Microsoft::WRL::ComPtr;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
/// <param name="sceneManager">登録されているシーンマネージャー</param>
SceneStageSelect::SceneStageSelect(SceneManager * sceneManager)
	: SceneBase(sceneManager),
	  m_showFlag(false),
	  m_selectSceneID(0)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SceneStageSelect::~SceneStageSelect()
{
}

/// <summary>
/// ロゴシーンの初期化処理
/// </summary>
void SceneStageSelect::Initialize()
{
	m_toPlayMoveOnChecker = false;

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();

	// カメラオブジェクトの作成
	mp_camera = std::make_unique<GameCamera>(size.right, size.bottom);

	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelRoadStraight = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_straight.cmo", fx);
	m_modelRoadStop = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_stop.cmo", fx);
	m_modelRoadCurve = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_curve.cmo", fx);
	m_modelRoadBranch = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\road_branch.cmo", fx);

}

/// <summary>
/// ロゴシーンの終了処理
/// </summary>
void SceneStageSelect::Finalize()
{
}

/// <summary>
/// ステージ選択シーンの更新処理
/// </summary>
/// <param name="timer">時間情報</param>
void SceneStageSelect::Update(DX::StepTimer const& timer)
{
	// カメラの更新(プレイヤー情報は無し)
	mp_camera->Update(timer, nullptr);

	// 入力情報を更新
	InputManager::SingletonGetInstance().Update();

	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Up))
	{
		m_selectSceneID++;
		m_showFlag = false;
		if (m_selectSceneID > 2)m_selectSceneID = 0;
		SceneManager::SetStageNum(m_selectSceneID);
	}

	// キー入力
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyPressed(DirectX::Keyboard::Space))
	{
		m_toPlayMoveOnChecker = true;
	}
	if (m_toPlayMoveOnChecker == true)
	{
		m_sceneManager->RequestToChangeScene(SCENE_PLAY);
	}
}

/// <summary>
/// ロゴシーンの描画処理
/// </summary>
void SceneStageSelect::Render()
{
	// ビュー行列の作成
	DirectX::SimpleMath::Matrix view = DirectX::SimpleMath::Matrix::CreateLookAt(mp_camera->GetEyePosition(), mp_camera->GetTargetPosition(), DirectX::SimpleMath::Vector3::Up);

	// ウインドウサイズからアスペクト比を算出する
	RECT size = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	// 画角を設定
	float fovAngleY = XMConvertToRadians(45.0f);

	// 射影行列を作成
	SimpleMath::Matrix projection = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		200.0f
	);

	// 行列を設定
	MatrixManager::SingletonGetInstance().SetViewProjection(view, projection);

	// デバッグ用
	GameDebug::SingletonGetInstance().DebugRender("SceneStageSelect", DirectX::SimpleMath::Vector2(20.0f, 10.0f));

	if (!m_showFlag)
	{
		/*switch (SceneManager::GetStageNum())
		{
		case 1:
			LoadStage(m_selectSceneID);
			GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			m_showFlag = true;
			break;
		case 2:
			LoadStage(m_selectSceneID);
			GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			m_showFlag = true;
			break;
		default:
			GameDebug::SingletonGetInstance().DebugRender("StageNone", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			break;
		}*/
		if (m_selectSceneID == 0)
		{
			GameDebug::SingletonGetInstance().DebugRender("StageNone", DirectX::SimpleMath::Vector2(20.0f, 30.0f));
		}
		else
		{
			LoadStage(m_selectSceneID);
			GameDebug::SingletonGetInstance().DebugRender("Stage", float(SceneManager::GetStageNum()), DirectX::SimpleMath::Vector2(20.0f, 30.0f));
			m_showFlag = true;
		}
	}
	
	GameDebug::SingletonGetInstance().Render();
	if (m_showFlag)
	{
		ShowStage();
	}
}

/// <summary>
/// 選択しているステージを表示
/// </summary>
/// <param name="stageID">ステージID</param>
void SceneStageSelect::LoadStage(int stageID)
{
	// ステージマップの読み込み
	std::string filePath = "Resources\\StageMap\\Stage";
	std::ostringstream os;
	int stageNum = m_selectSceneID;
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

	/*// マップの行数を読み込む
	while (getline(ifs, line))
	{
		std::istringstream stream(line);
		std::string buf;
		// マップの列数を読み込む
		while (getline(wifs, line))
		{

		}
	}
	
	// 行配列を確保する
	map.resize(mapRow);
	// 列配列を確保する
	for (int row = 0; row < mapRow; row++)
	{
		map[row].resize(mapColum);
	}*/

	// 
	m_roadObject.resize(m_maxFloorBlock);
	for (int j = 0; j < m_maxFloorBlock; j++)
	{
		for (int i = 0; i < m_maxFloorBlock; i++)
		{
			m_roadObject[i].resize(m_maxFloorBlock);
		}
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
/// 選択しているステージを表示
/// </summary>
void SceneStageSelect::ShowStage()
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
			// 描画道路選択
			int roadType = m_roadObject[j][i].roadType;
			switch (roadType)
			{
			case 0: break;                                                                                                                  // 何もなし
			case 1: m_modelRoadStraight->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());  break;   // 直線道路
			case 2: m_modelRoadStop->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());      break;   // 末端道路
			case 3: m_modelRoadCurve->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());     break;   // 曲線道路
			case 4: m_modelRoadBranch->Draw(res.GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());    break;   // 分岐道路
			}
		}
	}


}
