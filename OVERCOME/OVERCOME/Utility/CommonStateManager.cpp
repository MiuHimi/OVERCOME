//////////////////////////////////////////////////////////////
// File.    CommonStateManager.cpp
// Summary. CommonStateManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../pch.h"
#include "CommonStateManager.h"
#include "DeviceResources.h"

/// <summary>
/// コンストラクタ
/// </summary>
CommonStateManager::CommonStateManager()
{
}
/// <summary>
/// デストラクタ
/// </summary>
CommonStateManager::~CommonStateManager()
{
}

/// <summary>
/// コモンステートを取得する
/// </summary>
/// <returns>コモンステート</returns>
DirectX::CommonStates * CommonStateManager::GetStates()
{
	ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();
	// コモンステートの作成
	m_states = std::make_unique<DirectX::CommonStates>(device);

	return m_states.get();
}
