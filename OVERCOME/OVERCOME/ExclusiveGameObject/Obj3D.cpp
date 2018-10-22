/*
	Obj3D.cpp
*/

// ヘッダをインクルード
#include "../pch.h"
#include "../Utility/CommonStateManager.h"

#include "Obj3D.h"
#include "../Game.h"

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
Obj3D::Obj3D():mp_model(nullptr), mp_game(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Obj3D::~Obj3D()
{
}

/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">実行時間</param>
/// <returns>終了状態</returns>
bool Obj3D::Update(float elapsedTime)
{
	// ワールド行列の作成
	m_world = Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);

	return true;
}

/// <summary>
/// 描画
/// </summary>
void Obj3D::Render()
{
	// モデルの描画
	if (mp_model && mp_game) // ポインタがnullでない場合(nullの場合if文に入らない)
	{
		// モデルの描画
		mp_model->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
			           m_world, mp_game->GetView(), mp_game->GetProjection());
	}
}
