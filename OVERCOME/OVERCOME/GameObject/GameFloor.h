//////////////////////////////////////////////////////////////
// File.    GameFloor.h
// Summary. GameFloorClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class Game;
class GameFloor : public CollisionBox
{
// �����o�[�ϐ�(�\���́Aenum�A�񋓎q etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelLattice;   // �i�q�������f���n���h��
	//std::unique_ptr<CollisionBox> mp_floor;         // ���Փ˔���p�I�u�W�F�N�g

	Game* mp_game;                                    // Game�t�@�C���̏����i�[

// �����o�[�֐�(�֐��AGetter�ASetter)
public:
	// �R���X�g���N�^
	GameFloor(Game* game);
	// �f�X�g���N�^
	~GameFloor();

	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer, Game* game);
	// �`��
	void Render(Game* game, DirectX::SimpleMath::Matrix view);
	// �p������
	void Depose();

private:


};