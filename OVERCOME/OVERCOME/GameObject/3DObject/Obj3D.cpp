/*
	Obj3D.cpp
*/

// �w�b�_���C���N���[�h
#include "../../pch.h"
#include "../../Utility/CommonStateManager.h"
#include "../../Utility/MatrixManager.h"

#include "Obj3D.h"
#include "../../Game.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Obj3D::Obj3D():mp_model(nullptr)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Obj3D::~Obj3D()
{
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="elapsedTime">���s����</param>
/// <returns>�I�����</returns>
bool Obj3D::Update(float elapsedTime)
{
	// ���[���h�s��̍쐬
	m_world = Matrix::CreateFromQuaternion(m_rotation) * Matrix::CreateTranslation(m_position);

	return true;
}

/// <summary>
/// �`��
/// </summary>
void Obj3D::Render(MatrixManager* matrixManager)
{
	// ���f���̕`��
	if (mp_model) // �|�C���^��null�łȂ��ꍇ(null�̏ꍇif���ɓ���Ȃ�)
	{
		// ���f���̕`��
		mp_model->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
			           m_world, matrixManager->GetView(), matrixManager->GetProjection());
	}
}