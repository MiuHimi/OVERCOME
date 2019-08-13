//////////////////////////////////////////////////////////////
// File.    Obj2D.h
// Summary. Obj2DClass,BaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class Obj2D
{
// �����o�[�ϐ�
protected:
	// �ʒu
	DirectX::SimpleMath::Vector2 m_position;

	// ���A����
	float m_width, m_height;

// �����o�[�֐�
public:
	// �R���X�g���N�^
	Obj2D();
	// �f�X�g���N�^
	~Obj2D();

	// �X�V
	virtual bool Update(float elapsedTime);
	// �`��
	virtual void Render();

	//-----------------------------------Setter-----------------------------------//

	// �ʒu��ݒ�
	void SetPosition(const DirectX::SimpleMath::Vector2 position) { m_position = position; }
	// ���A������ݒ�
	void SetWidthHeight(const float width, const float height)    { m_width = width, m_height = height; }
	//----------------------------------------------------------------------------//

};