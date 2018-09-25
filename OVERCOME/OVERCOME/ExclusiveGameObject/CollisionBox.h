/*
	CollisionBox.h
	���E����������Obj3D�N���X
*/

#pragma once

#include "Obj3D.h"
#include "Collision.h"
#include "DebugBox.h"

// ���Փ˔���N���X
class CollisionBox : public Obj3D
{
private:
	// �Փ˔�����
	Collision::Box m_collision;

	// �Փ˔���\���f�o�b�O�I�u�W�F�N�g
	std::unique_ptr<DebugBox> m_dbgObj;

public:
	// Setter---
	// �Փ˔�����̐ݒ�
	void SetCollision(Collision::Box box);
	//---//

	// Getter---
	// �Փ˔�����̎擾
	Collision::Box GetCollision();
	//---//
	
	// �f�o�b�O�p�I�u�W�F�N�g�\���֐�
	void DrawDebugCollision(DirectX::SimpleMath::Matrix view);
	void DrawDebugCollision(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view);

};