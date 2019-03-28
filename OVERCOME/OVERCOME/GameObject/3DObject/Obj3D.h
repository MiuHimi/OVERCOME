/*
	Obj3D.h
*/

#pragma once

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../pch.h"

class Game;
class MatrixManager;
class Obj3D
{
// �����o�[�ϐ�
protected:
	// ���f���n���h��
	DirectX::Model* mp_model;

	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;
	// ��]
	DirectX::SimpleMath::Quaternion m_rotation;

	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;

// �����o�[�֐�
public:
	// �R���X�g���N�^
	Obj3D();
	// �f�X�g���N�^
	~Obj3D();

	// �X�V
	virtual bool Update(float elapsedTime);
	// �`��
	virtual void Render(MatrixManager* matrixManager);


	// Setter
	//---
	// ���f���n���h����ݒ�
	void SetModel(DirectX::Model* model)                             { mp_model = model; }

	// �ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3 position)    { m_position = position; }
	// ��]��ݒ�
	void SetRotation(const DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	//---//

};