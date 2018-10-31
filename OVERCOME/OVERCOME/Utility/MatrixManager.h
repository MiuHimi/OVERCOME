//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/31
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
	DirectX::SimpleMath::Matrix   m_projection;   // �ˉe�s��

// �����o�[�֐�
public:
	// �f�X�g���N�^
	~MatrixManager();

	// �t�����h�֐�
	friend SingletonBase<MatrixManager>;

	// ���Z�b�g
	void ResetMatrix();
	

	// Getter
	DirectX::SimpleMath::Matrix& GetView()       { return m_view; }
	DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// Setter
	void SetView(DirectX::SimpleMath::Matrix& view)             { m_view = view; }
	void SetProjection(DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }

private:
	// �R���X�g���N�^
	MatrixManager();

};