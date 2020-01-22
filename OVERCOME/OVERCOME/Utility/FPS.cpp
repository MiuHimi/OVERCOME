//////////////////////////////////////////////////////////////
// File.    FPS.cpp
// Summary. ControlFPSClass
// Date.    2020/01/10
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "FPS.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
FPS::FPS() : m_startTime(0),m_flameCount(0),m_fps(0)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
FPS::~FPS()
{
}

/// <summary>
/// FPS�v���p�X�V�֐�
/// </summary>
/// <returns></returns>
bool FPS::FPSUpdate()
{
	//1�t���[���ڂȂ玞�����L��
	if (m_flameCount == 0) m_startTime = GetCurrentTime();

	if (m_flameCount == SAMPLE_FPS)
	{
		//60�t���[���ڂȂ畽�ς��v�Z����
		int t = GetCurrentTime();
		m_fps = 1000.f / ((t - m_startTime) / (float)SAMPLE_FPS);
		m_flameCount = 0;
		m_startTime = t;
	}

	// �J�E���g�X�V
	m_flameCount++;
	return true;
}

/// <summary>
/// FPS����pSleep�֐�
/// </summary>
void FPS::FPSWait()
{
	// �����������Ԃ��v��
	int tookTime = GetCurrentTime() - m_startTime;
	// ��������҂ׂ����Ԃ��Z�o
	int waitTime = m_flameCount * 1000 / SET_FPS - tookTime;
	// �Z�o�����������ҋ@
	if (waitTime > 0) Sleep(waitTime);
}
