//////////////////////////////////////////////////////////////
// File.    CollisionBox.cpp
// Summary. ���E����������Obj3D�N���X
// Date.    2018/10/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �w�b�_���C���N���[�h
#include "../pch.h"
#include "../Game.h"
#include "CollisionBox.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"


/// <summary>
/// �Փ˔�����̐ݒ�
/// </summary>
/// <param name="sphere">�Փ˔�����</param>
void CollisionBox::SetCollision(Collision::Box box)
{
	// �Փ˔������ݒ�
	m_collision = box;

	// �f�o�b�O�p���f���̍쐬
	m_dbgObj = std::make_unique<DebugBox>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), m_collision.c, m_collision.r);
}

/// <summary>
/// �Փ˔�����̎擾
/// </summary>
/// <returns>�Փ˔��蔠���</returns>
Collision::Box CollisionBox::GetCollision()
{// �u���[�J���ȍ��W�����[���h���W�n�ɕϊ��v
	Collision::Box box;

	// ���E���̒��S���W�����[���h�s��ɍ��W�ϊ�����
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	box.c = DirectX::SimpleMath::Vector3::Transform(m_collision.c, world);
	box.r = m_collision.r;

	return box;
}

/// <summary>
/// �f�o�b�O�p�I�u�W�F�N�g�\���֐�
/// </summary>
void CollisionBox::DrawDebugCollision()
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// �f�o�b�O�p�I�u�W�F�N�g�̕\��
	m_dbgObj->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());
}
