//////////////////////////////////////////////////////////////
// File.    MatrixManager.cpp
// Summary. MatrixManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "MatrixManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
MatrixManager::MatrixManager()
{
}
/// <summary>
/// デストラクタ
/// </summary>
MatrixManager::~MatrixManager()
{
}

/// <summary>
/// 行列初期化
/// </summary>
void MatrixManager::ResetMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::Identity;
	m_projection = DirectX::SimpleMath::Matrix::Identity;
}
