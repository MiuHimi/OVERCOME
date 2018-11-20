//////////////////////////////////////////////////////////////
// File.    CountManager.cpp
// Summary. CountManagerClass(Frame Count)
// Date.    2018/11/20
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "CountManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
CountManager::CountManager()
	: m_count(0),
	  m_countFlag(false)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
CountManager::~CountManager()
{
}
/// <summary>
/// �X�V
/// </summary>
void CountManager::Update()
{

}

/// <summary>
/// �J�E���g���J�n
/// </summary>
void CountManager::CountStart()
{
	m_countFlag = true;
}
/// <summary>
/// �J�E���g���Ă���
/// </summary>
void CountManager::Counting()
{
	if (m_countFlag)m_count++;
}
/// <summary>
/// �J�E���g�����Z�b�g
/// </summary>
void CountManager::CountReset()
{
	m_count = 0;
}
/// <summary>
/// �J�E���g��(�t���[����)���擾
/// </summary>
/// <returns>�t���[����</returns>
int CountManager::GetCount()
{
	return m_count;
}
/// <summary>
/// FPS���擾
/// </summary>
/// <returns>FPS</returns>
int CountManager::GetFPS()
{
	return m_fps;
}
