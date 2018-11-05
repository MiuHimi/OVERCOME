//////////////////////////////////////////////////////////////
// File.    GameCamera.h
// Summary. GameCameraClass
// Date.    2018/10/05
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

#include "Camera.h"

#include "Player.h"

class Player;
class GameCamera : public Camera
{
// �����o�[�ϐ�
public:

private:
	float          m_angle;          // ��]�p

	// �J�����̋���
	static const float DEFAULT_CAMERA_DISTANCE;

	// ����]
	float m_yAngle, m_yTmp;
	// �c��]
	float m_xAngle, m_xTmp;

	int m_posX, m_posY;  // �񎟌���̐�Βl(���W)

	// �h���b�O���ꂽ���W
	
	float m_sx, m_sy;

	// �X�N���[���t�H�C�[���l
	int m_scrollWheelValue;


// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameCamera();
	GameCamera(int windowWidth, int windowHeight);
	// �f�X�g���N�^
	~GameCamera();

	// �X�V(��ɃJ�����̐؂�ւ����s��)
	bool Update(DX::StepTimer const & timer, Player* player);

	// ���_�𒍎��_�ɂ��A��������񂷂�J����
	void OriginPointAroundCamera();

	// �����Ă���Ƃ��p�J����
	void RunPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);
	// ��납��ǂ�������J����
	void FollowPlayerCamera(DirectX::SimpleMath::Vector3 target, float direction);

	// �}�E�X�Ŏ��_�ړ�����J����
	void MouseOperateCamera();

	/// <summary>
	/// ��ʃT�C�Y�̐ݒ�֐�
	/// </summary>
	/// <param name="windowWidth">�E�C���h�E�T�C�Y�i���j</param>
	/// <param name="windowHeight">�E�C���h�E�T�C�Y�i�����j</param>
	void SetWindowSize(int windowWidth, int windowHeight);

private:

	void Motion(int x, int y);

};
