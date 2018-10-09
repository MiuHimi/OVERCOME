//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/09
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

class MatrixManager
{
// �����o�[�ϐ�
public:

private:
	static DirectX::SimpleMath::Matrix              m_world;                // ���[���h�s��
	static DirectX::SimpleMath::Matrix              m_view;                 // �r���[�s��
	static DirectX::SimpleMath::Matrix              m_projection;           // �ˉe�s��

// �����o�[�֐�
public:
	// Setter
	static void SetWorldMatrix(DirectX::SimpleMath::Matrix world)           { m_world = world; }
	static void SetViewMatrix(DirectX::SimpleMath::Matrix view)             { m_view = view; }
	static void SetProjectionMatrix(DirectX::SimpleMath::Matrix projection) { m_projection = projection; }

	// Getter
	static DirectX::SimpleMath::Matrix GetWorldMatrix()                     { return m_world; }
	static DirectX::SimpleMath::Matrix GetViewMatrix()                      { return m_view; }
	static DirectX::SimpleMath::Matrix GetProjectionMatrix()                { return m_projection; }

private:

};