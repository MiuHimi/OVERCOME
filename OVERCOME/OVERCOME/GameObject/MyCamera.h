//////////////////////////////////////////////////////////////
// File.    MyCamera.h
// Summary. MyCameraClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

#include "Camera.h"

#include "Player.h"

class Player;
class MyCamera : public Camera
{
// �����o�[�ϐ�
public:

private:
	float          m_angle;          // ��]�p

// �����o�[�֐�
public:
	// �R���X�g���N�^
	MyCamera();
	// �f�X�g���N�^
	~MyCamera();

	// �X�V(��ɃJ�����̐؂�ւ����s��)
	bool Update(DX::StepTimer const & timer, Player* player);

	// ���_�𒍎��_�ɂ��A��������񂷂�J����
	void OriginPointAroundCamera();

	// �����Ă���Ƃ��p�J����
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// ��납��ǂ�������J����
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

private:

};