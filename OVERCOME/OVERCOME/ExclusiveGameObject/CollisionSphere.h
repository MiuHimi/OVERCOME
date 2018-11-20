/*
	CollisionSphere.h
	���E����������Obj3D�N���X
*/

#pragma once

#include "Obj3D.h"
#include "Collision.h"
#include "DebugSphere.h"

// ���Փ˔���N���X
class CollisionSphere : public Obj3D
{
private:
	// �Փ˔�����
	Collision::Sphere m_collision;

	// �Փ˔���\���f�o�b�O�I�u�W�F�N�g
	std::unique_ptr<DebugSphere> m_dbgObj;

public:
	// Setter---
	// �Փ˔�����̐ݒ�
	void SetCollision(Collision::Sphere sphere);
	//---//

	// Getter---
	// �Փ˔�����̎擾
	Collision::Sphere GetCollision();
	//---//

	// �f�o�b�O�p�I�u�W�F�N�g�\���֐�
	void DrawDebugCollision();

};