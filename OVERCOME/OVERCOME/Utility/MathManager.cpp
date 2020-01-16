//////////////////////////////////////////////////////////////
// File.    InputManager.cpp
// Summary. InputManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include <math.h>

#include "../pch.h"
#include "MathManager.h"

// using�f�B���N�g��


/// <summary>
/// �R���X�g���N�^
/// </summary>
MathManager::MathManager()
{
}

/// <summary>
/// ��_�Ԃ̋��������߂ĕԂ�(2D)
/// �������͋��߂Ȃ�
/// </summary>
/// <param name="x">�n�_A:X���W</param>
/// <param name="y">�n�_A:Y���W</param>
/// <param name="x2">�n�_B:X���W</param>
/// <param name="y2">�n�_B:Y���W</param>
/// <returns>����(float)</returns>
float MathManager::GetDistancePoints2D(float x, float y, float x2, float y2)
{
	float dist = (x2 - x) * (x2 - x) + (y2 - y) * (y2 - y);

	return dist;
}

/// <summary>
/// ��_�Ԃ̋��������߂ĕԂ�(3D)
/// �������͋��߂Ȃ�
/// </summary>
/// <param name="x">�n�_A:X���W</param>
/// <param name="y">�n�_A:Y���W</param>
/// <param name="z">�n�_A:Z���W</param>
/// <param name="x2">�n�_B:X���W</param>
/// <param name="y2">�n�_B:Y���W</param>
/// <param name="z2">�n�_B:Z���W</param>
/// <returns>����(float)</returns>
float MathManager::GetDistancePoints3D(float x, float y, float z, float x2, float y2, float z2)
{
	float dist = ((x - x2)*(x - x2)) +
				 ((y - y2)*(y - y2)) +
				 ((z - z2)*(z - z2));

	return dist;
}
