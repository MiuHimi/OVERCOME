//////////////////////////////////////////////////////////////
// File.    KeyManager.h
// Summary. KeyManagerClass
// Date.    2018/08/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��


class KeyManager
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	//static int   m_aTrigCount[0xff + 1];  //< VK_xx�̐������p��

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	KeyManager();
	// �f�X�g���N�^
	virtual ~KeyManager();

	// �L�[���͏����Z�b�g
	static void SetTrigger(int key);

	// �L�[���͏������Z�b�g�A���t���[���Ăяo��
	static void AllClear();

	// �L�[���͏����擾
	static bool IsTrigger(int key);
};
