//////////////////////////////////////////////////////////////
// File.    CommonStateManager.cpp
// Summary. CommonStateManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "CommonStateManager.h"
#include "DeviceResources.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CommonStateManager::CommonStateManager()
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
CommonStateManager::~CommonStateManager()
{
}

/// <summary>
/// �R�����X�e�[�g���擾����
/// </summary>
/// <returns>�R�����X�e�[�g</returns>
DirectX::CommonStates * CommonStateManager::GetStates()
{
	ID3D11Device* device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();
	// �R�����X�e�[�g�̍쐬
	m_states = std::make_unique<DirectX::CommonStates>(device);

	return m_states.get();
}
