//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "SingletonBase.h"

class MatrixManager : public SingletonBase<MatrixManager>
{
// �����o�[�ϐ�
public:

private:
	DirectX::SimpleMath::Matrix   m_view;         // �r���[�s��
	DirectX::SimpleMath::Matrix   m_projection;   // �r���[�s��

// �����o�[�֐�
public:
	// �f�X�g���N�^
	~MatrixManager();

	// �t�����h�֐�
	friend SingletonBase<MatrixManager>;

	// Getter
	DirectX::SimpleMath::Matrix& GetView();
	DirectX::SimpleMath::Matrix& GetProjection();

	// Setter
	DirectX::SimpleMath::Matrix SetView(DirectX::SimpleMath::Matrix view);
	DirectX::SimpleMath::Matrix SetProjection(DirectX::SimpleMath::Matrix projection);

private:
	// �R���X�g���N�^
	MatrixManager();

};