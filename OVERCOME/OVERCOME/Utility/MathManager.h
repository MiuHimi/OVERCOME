//////////////////////////////////////////////////////////////
// File.    MathManager.h
// Summary. MathManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

#include "SingletonBase.h"

class MathManager : public SingletonBase<MathManager>
{
// �����o�[�ϐ�
//public:

//private:

// �����o�[�֐�
private:
	// �t�����h�֐�
	friend class SingletonBase<MathManager>;

	// �R���X�g���N�^
	MathManager();

public:
	// ��_�Ԃ̋��������߂ĕԂ�(�񎟌�)
	float GetDistancePoints2D(float x, float y, float x2, float y2);

	// ��_�Ԃ̋��������߂ĕԂ�(�O����)
	float GetDistancePoints3D(float x, float y, float x2, float y2, float x3, float y3);

};