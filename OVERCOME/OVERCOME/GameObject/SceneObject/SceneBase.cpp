//////////////////////////////////////////////////////////////
// File.    SceneBase.cpp
// Summary. SceneBassClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "SceneBase.h"
#include "SceneManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="sceneManager">�o�^����Ă���V�[���}�l�[�W���[</param>
SceneBase::SceneBase(SceneManager* sceneManager)
	: m_sceneManager(sceneManager)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneBase::~SceneBase()
{
}