//////////////////////////////////////////////////////////////
// File.    MatrixManager.cpp
// Summary. MatrixManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "MatrixManager.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MatrixManager::MatrixManager()
	: m_view(DirectX::SimpleMath::Matrix::Identity)
	, m_projection(DirectX::SimpleMath::Matrix::Identity)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
MatrixManager::~MatrixManager()
{
}

/// <summary>
/// �s�񏉊���
/// </summary>
void MatrixManager::ResetMatrix()
{
	m_view = DirectX::SimpleMath::Matrix::Identity;
	m_projection = DirectX::SimpleMath::Matrix::Identity;
}
