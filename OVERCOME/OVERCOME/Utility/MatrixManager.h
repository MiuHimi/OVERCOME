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
	static DirectX::SimpleMath::Matrix   m_view;         // �r���[�s��
	static DirectX::SimpleMath::Matrix   m_projection;   // �ˉe�s��

// �����o�[�֐�
public:
	// �f�X�g���N�^
	~MatrixManager();

	// �t�����h�֐�
	friend SingletonBase<MatrixManager>;

	// ���Z�b�g
	void ResetMatrix();
	

	// Getter
	static DirectX::SimpleMath::Matrix& GetView()       { return m_view; }
	static DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// Setter
	static void SetView(DirectX::SimpleMath::Matrix& view)             { m_view = view; }
	static void SetProjection(DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }

private:
	// �R���X�g���N�^
	MatrixManager();

};