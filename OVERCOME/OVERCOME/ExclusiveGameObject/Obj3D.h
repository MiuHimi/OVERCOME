/*
	Obj3D.h
*/

#pragma once

#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../pch.h"

class Game;
class Obj3D
{
// �����o�[�ϐ�
protected:
	Game* mp_game;

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
	virtual void Render();


	// Setter
	//---
	// �Q�[���I�u�W�F�N�g��ݒ�
	void SetGame(Game* game)                                         { mp_game = game; }
	// ���f���n���h����ݒ�
	void SetModel(DirectX::Model* model)                             { mp_model = model; }

	// �ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector3 position)    { m_position = position; }
	// ��]��ݒ�
	void SetRotation(const DirectX::SimpleMath::Quaternion rotation) { m_rotation = rotation; }
	//---//

};