//////////////////////////////////////////////////////////////
// File.    CommonStateManager.h
// Summary. CommonStateManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"

#include <CommonStates.h>

#include "SingletonBase.h"

class CommonStateManager : public SingletonBase<CommonStateManager>
{
// �����o�[�ϐ�
public:
	std::unique_ptr<DirectX::CommonStates>   m_states;    // �R�����X�e�[�g

private:

// �����o�[�֐�
public:
	// �f�X�g���N�^
	~CommonStateManager();

	// �t�����h�֐�
	friend SingletonBase<CommonStateManager>;

	// �R�����X�e�[�g���擾����
	DirectX::CommonStates* GetStates();

private:
	// �R���X�g���N�^
	CommonStateManager();

};
