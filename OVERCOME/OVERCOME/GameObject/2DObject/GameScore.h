//////////////////////////////////////////////////////////////
// File.    GameScore.h
// Summary. GameScoreClass
// Date.    2019/9/11
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <WICTextureLoader.h>
#include <CommonStates.h>

#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"
#include "Obj2D.h"

class GameScore
{
// �����o�[�ϐ�
//public:

private:
	static int								m_score;			// �v���C���[�����链�_

    // �X�R�A�̌�
	enum Digit
	{
		THOUSAND,
		HUNDRED,
		TEN,
		ONE,

		NUM
	};

	bool									m_isFullScreen;		// �t���X�N���[�����ǂ���

	float									m_basePosX;			// �X�R�A�\����ʒu

	std::unique_ptr<Obj2D>					mp_scoreStr;		// �X�R�A�I�u�W�F�N�g
	std::unique_ptr<Obj2D>					mp_scoreBG;			// �X�R�A�w�i�I�u�W�F�N�g

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameScore();
	// �f�X�g���N�^
	~GameScore();

	// ����
	void Create(const bool isFullScreen, const wchar_t* scoreFileName, const wchar_t* backgroundFileName);
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

//private:

};