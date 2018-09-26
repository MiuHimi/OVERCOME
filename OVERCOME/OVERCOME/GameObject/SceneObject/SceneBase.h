//////////////////////////////////////////////////////////////
// File.    SceneBase.h
// Summary. SceneBaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../../DeviceResources.h"
#include "../../StepTimer.h"

#include "../../Game.h"

#include "../../ExclusiveGameObject/InputManager.h"

class SceneManager;
class SceneBase
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

protected:
	SceneManager* m_sceneManager;   // ���N���X�ւ̃|�C���^

private:

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	SceneBase(SceneManager* sceneManager);
	// �f�X�g���N�^
	virtual ~SceneBase();

	// ������
	virtual void Initialize() = 0;
	// �X�V
	//virtual void Update(DX::StepTimer const& timer) = 0;
	virtual void Update(DX::StepTimer const& timer, Game* game) = 0;
	// �`��
	//virtual void Render() = 0;
	virtual void Render(DirectX::SpriteBatch* sprite, Game* game) = 0;
	// �I��
	virtual void Finalize() = 0;
};