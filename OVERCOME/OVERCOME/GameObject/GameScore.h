//////////////////////////////////////////////////////////////
// File.    GameScore.h
// Summary. GameScoreClass
// Date.    2018/10/02
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <WICTextureLoader.h>
#include <CommonStates.h>

#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

class GameScore
{
// �����o�[�ϐ�
public:

private:
	int m_score;
	int m_addScore;
	int m_count;

	std::unique_ptr<DirectX::SpriteBatch>                m_sprites;                     // �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::CommonStates>               m_states;                      // �R�����X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_texture;                     // �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureNum[10];              // �e�N�X�`���n���h��

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameScore();
	// �f�X�g���N�^
	~GameScore();

	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render();

	// Getter,Setter
	int GetScore() { return m_score; }
	void SetScore(int score) { m_score = score; }
	void FluctuationScore(int addscore) { m_score += addscore; }

private:

};