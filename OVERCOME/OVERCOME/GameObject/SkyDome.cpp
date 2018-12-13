//////////////////////////////////////////////////////////////
// File.    SkyDome.cpp
// Summary. SkyDomeClass
// Date.    2018/09/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "SkyDome.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="game">ゲームオブジェクト</param>
SkyDome::SkyDome()
{
}
/// <summary>
/// デストラクタ
/// </summary>
SkyDome::~SkyDome()
{
	//SkyDome::Depose();
}

/// <summary>
/// 初期化処理
/// </summary>
void SkyDome::Initialize()
{
}
/// <summary>
/// 生成処理
/// </summary>
void SkyDome::Create()
{
	// エフェクトファクトリー
	EffectFactory fx(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
	// モデルのテクスチャの入っているフォルダを指定する
	fx.SetDirectory(L"Resources\\Models");
	// モデルをロードしてモデルハンドルを取得する
	m_modelSky = Model::CreateFromCMO(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Models\\sky2.cmo", fx);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="timer">起動経過時間</param>
/// <returns>終了状態</returns>
bool SkyDome::Update(DX::StepTimer const & timer)
{
	return true;
}
/// <summary>
/// 描画処理
/// </summary>
void SkyDome::Render()
{
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix trans = SimpleMath::Matrix::Identity;
	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;

	// 床の描画
	world = SimpleMath::Matrix::Identity;
	/*m_modelSky->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		             world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());*/
}

/// <summary>
/// 削除処理
/// </summary>
void SkyDome::Depose()
{
}
