//////////////////////////////////////////////////////////////
// File.    FPS.h
// Summary. ControlFPSClass
// Date.    2020/01/10
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

class FPS
{
// �����o�[�ϐ�
//public:

private:
	int					m_startTime = 0;	// ����J�n����
	int					m_flameCount = 0;	// �t���[���J�E���^�[
	float				m_fps = 0;			// FPS�v���ϐ�
	static const int	SAMPLE_FPS = 60;	// ���ς����T���v����
	static const int	SET_FPS = 60;		// �ݒ肵����FPS

// �����o�[�ϐ�
public:
	// �R���X�g���N�^
	FPS();
	// �f�X�g���N�^
	~FPS();

	// FPS�v���p�X�V�֐�
	bool FPSUpdate();

	// FPS����pSleep�֐�
	void FPSWait();

	// FPS���擾
	float GetFPS() { return m_fps; }

//private:

};