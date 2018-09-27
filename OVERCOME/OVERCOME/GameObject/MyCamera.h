//////////////////////////////////////////////////////////////
// File.    MyCamera.h
// Summary. MyCameraClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

#include "Camera.h"

#include "Player.h"

class Player;
class MyCamera : public Camera
{
// �����o�[�ϐ�
public:

private:
	// ��]�p
	float m_angle;

	float m_count = 0.0f;

	Player* mp_player;

// �����o�[�֐�
public:
	// �R���X�g���N�^
	MyCamera();
	// �f�X�g���N�^
	~MyCamera();

	bool Update(DX::StepTimer const & timer, Player* player);

	// �^�C�g���V�[���p�J����
	void TitleSceneCamera();
	// �Q�[���V�[���p�J����
	void GameSceneCamera(DirectX::SimpleMath::Vector3 target, float direction);

	// �����Ă���Ƃ��p�J����
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// ��납��ǂ�������J����
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

private:

};