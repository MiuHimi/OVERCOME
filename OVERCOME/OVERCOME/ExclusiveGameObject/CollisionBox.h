//////////////////////////////////////////////////////////////
// File.    CollisionBox.h
// Summary. ���E����������Obj3D�N���X
// Date.    2018/10/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../GameObject/3DObject/Obj3D.h"
#include "Collision.h"
#include "DebugBox.h"

// ���Փ˔���N���X
class MatrixManager;
class CollisionBox : public Obj3D
{
// �����o�[�ϐ�
public:

private:
	Collision::Box                m_collision;     // �Փ˔�����

	std::unique_ptr<DebugBox>     m_dbgObj;        // �Փ˔���\���f�o�b�O�I�u�W�F�N�g

// �����o�[�֐�
public:
	// �f�o�b�O�p�I�u�W�F�N�g�\���֐�
	void DrawDebugCollision(MatrixManager* matrixManager);

	// �Փ˔�����̐ݒ�
	void SetCollision(Collision::Box box);
	// �Փ˔�����̎擾
	Collision::Box GetCollision();

private:

};