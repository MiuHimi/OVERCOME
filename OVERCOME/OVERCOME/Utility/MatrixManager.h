//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��

class MatrixManager
{
// �����o�[�ϐ�
public:

private:
	DirectX::SimpleMath::Matrix   m_view;         // �r���[�s��
	DirectX::SimpleMath::Matrix   m_projection;   // �ˉe�s��

// �����o�[�֐�
public:
	// �R���X�g���N�^
	MatrixManager();
	// �f�X�g���N�^
	~MatrixManager();

	// ���Z�b�g
	void ResetMatrix();	

	// Getter
	DirectX::SimpleMath::Matrix& GetView()       { return m_view; }
	DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// Setter
	void SetViewProjection(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& projection) { m_view = view; m_projection = projection; }

private:

};