//////////////////////////////////////////////////////////////
// File.    CountManager.h
// Summary. CountManagerClass(Frame Count)
// Date.    2018/11/20
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../Utility/SingletonBase.h"

class CountManager : public SingletonBase<CountManager>
{
// �����o�[�ϐ�
public:
	int           m_count;         // �J�E���g(�t���[����)
	bool          m_countFlag;     // �J�E���g���J�n���ꂽ��t���O������
	const int     m_fps = 60;      // 60�t���[���ň�b

private:

// �����o�[�֐�
public:
	// �f�X�g���N�^
	~CountManager();

	// �X�V
	void Update();

	// �t�����h�֐�
	friend SingletonBase<CountManager>;

	// �J�E���g���J�n
	void CountStart();
	// �J�E���g���Ă���
	void Counting();
	// �J�E���g�����Z�b�g
	void CountReset();

	// �J�E���g(�t���[����)���擾
	int GetCount();
	// FPS���擾
	int GetFPS();

private:
	// �R���X�g���N�^
	CountManager();

};