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

#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"

class GameScore
{
// �����o�[�ϐ�
public:

private:
	static int                                         m_score;                // �v���C���[�����链�_

    // �X�R�A�̌�
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	DirectX::SimpleMath::Vector2					   m_scorePos;			   // �X�R�A�̕\���ʒu
	DirectX::SimpleMath::Vector2					   m_scoreBGPos;		   // �X�R�A�w�i�̕\���ʒu

	float											   m_scoreWidth;		   // �X�R�A�̕�
	float											   m_scoreHeight;		   // �X�R�A�̍���
	float											   m_scoreBGWidth;		   // �X�R�A�w�i�̕�
	float											   m_scoreBGHeight;		   // �X�R�A�w�i�̍���

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_textureBackground;   // �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   mp_texture;             // �e�N�X�`���n���h��
	std::unique_ptr<DirectX::SpriteBatch>              mp_sprite;			   // �X�v���C�g�o�b�`

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameScore();
	// �f�X�g���N�^
	~GameScore();

	// ����
	void Create(const wchar_t* scoreFileName, const wchar_t* backgroundFileName);
	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render();

	//-----------------------------------Getter-----------------------------------//

	static int GetScore()                           { return m_score; }
	//----------------------------------------------------------------------------//

	//-----------------------------------Setter-----------------------------------//

	static void SetScore(int score)                 { m_score = score; }
	//----------------------------------------------------------------------------//

	// ���_�̑���
	void FluctuationScore(int addscore)				{ m_score += addscore; }

private:

};