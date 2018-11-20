/*
	CollisionSphere.cpp
	���E����������Obj3D�N���X
*/

// �w�b�_���C���N���[�h
#include "../pch.h"
#include "../Game.h"
#include "CollisionSphere.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/MatrixManager.h"

/// <summary>
/// �Փ˔�����̐ݒ�
/// </summary>
/// <param name="sphere">�Փ˔�����</param>
void CollisionSphere::SetCollision(Collision::Sphere sphere)
{
	// �Փ˔������ݒ�
	m_collision = sphere;

	// �f�o�b�O�p���f���̍쐬
	m_dbgObj = std::make_unique<DebugSphere>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), m_collision.c, m_collision.r);

}

/// <summary>
/// �Փ˔�����̎擾
/// </summary>
/// <returns>�Փ˔��苅���</returns>
Collision::Sphere CollisionSphere::GetCollision()
{   // �u���[�J���ȍ��W�����[���h���W�n�ɕϊ��v
	Collision::Sphere sphere;

	// ���E���̒��S���W�����[���h�s��ɍ��W�ϊ�����
	sphere.c = DirectX::SimpleMath::Vector3::Transform(m_collision.c, m_world);
	sphere.r = m_collision.r;

	return sphere;
}

/// <summary>
/// �f�o�b�O�p�I�u�W�F�N�g�\���֐�
/// </summary>
void CollisionSphere::DrawDebugCollision()
{
	DirectX::SimpleMath::Matrix world = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);
	// �f�o�b�O�p�I�u�W�F�N�g�̕\��
	m_dbgObj->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(),
		           world, MatrixManager::SingletonGetInstance().GetView(), MatrixManager::SingletonGetInstance().GetProjection());

}
