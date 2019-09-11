//////////////////////////////////////////////////////////////
// File.    GameScore.cpp
// Summary. GameScoreClass
// Date.    2019/9/11
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../2DObject/GameScore.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScore::GameScore()
	: m_isFullScreen(false),
	  m_basePosX(0.0f),
	  mp_scoreStr(nullptr), mp_scoreBG(nullptr)
{	  
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameScore::~GameScore()
{
}

/// <summary>
/// ����
/// </summary>
void GameScore::Create(const bool isFullScreen, const wchar_t* scoreFileName, const wchar_t* backgroundFileName)
{
	// �e�ϐ��̏�����
	m_score = 0;

	// �t���X�N���[�����ǂ����擾
	m_isFullScreen = isFullScreen;

	// �A�N�e�B�u�ȃE�B���h�E�̃T�C�Y
	RECT activeWndRect;
	// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����擾
	HWND activeWnd = GetActiveWindow();
	// �A�N�e�B�u�ȃE�B���h�E�̃n���h�����炻�̉�ʂ̑傫�����擾
	GetWindowRect(activeWnd, &activeWndRect);

	// �E�B���h�E�̃T�C�Y���擾
	float windowWidth = float(activeWndRect.right) - float(activeWndRect.left);
	float windowHeight = float(activeWndRect.bottom) - float(activeWndRect.top);
	// �^�C�g���o�[�̍������擾
	int titlebarHeight = GetSystemMetrics(SM_CYCAPTION);

	// �E�B���h�E�̗]���ȕ���
	float excessSpace = 16.0f;

	// �X�R�A�w�i�I�u�W�F�N�g�̐���
	mp_scoreBG = std::make_unique<Obj2D>();
	mp_scoreBG->Create(backgroundFileName, nullptr);
	mp_scoreBG->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 200.0f, 80.0f, 1.0f, 1.0f);
	if (m_isFullScreen)
	{
		mp_scoreBG->SetPos(SimpleMath::Vector2((activeWndRect.right - activeWndRect.left) - mp_scoreBG->GetWidth(),
											   (activeWndRect.bottom - activeWndRect.top) - mp_scoreBG->GetHeight()));
	}
	else
	{
		mp_scoreBG->SetPos(SimpleMath::Vector2(float(activeWndRect.right - activeWndRect.left - 20.0f) - mp_scoreBG->GetWidth(),
											   float(activeWndRect.bottom - (activeWndRect.top + titlebarHeight + 20.0f)) - mp_scoreBG->GetHeight()));
	}
	mp_scoreBG->SetRect(0.0f, 0.0f, mp_scoreBG->GetWidth(), mp_scoreBG->GetHeight());

	// �X�R�A�I�u�W�F�N�g�̐���
	mp_scoreStr = std::make_unique<Obj2D>();
	mp_scoreStr->Create(scoreFileName, nullptr);
	mp_scoreStr->Initialize(SimpleMath::Vector2(0.0f, 0.0f), 40.0f, 60.0f, 1.0f, 1.0f);
	if (m_isFullScreen)
	{
		mp_scoreStr->SetPos(SimpleMath::Vector2(mp_scoreBG->GetPos().x + 10.0f,
												(activeWndRect.bottom - activeWndRect.top) - (10.0f + mp_scoreStr->GetHeight())));
	}
	else
	{
		mp_scoreStr->SetPos(SimpleMath::Vector2(mp_scoreBG->GetPos().x + 10.0f,
												float(activeWndRect.bottom - (activeWndRect.top + titlebarHeight+20.0f)) - float(10.0f + mp_scoreStr->GetHeight())));
	}
	mp_scoreStr->SetRect(0.0f, 0.0f, mp_scoreStr->GetWidth(), mp_scoreStr->GetHeight());

	// �X�R�A�\����ʒu�ݒ�
	m_basePosX = mp_scoreStr->GetPos().x;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
bool GameScore::Update(DX::StepTimer const& timer)
{	
	// �X�R�A�̉���
	if (GetScore() <= 0)
	{
		SetScore(0);
	}
	
	return true;
}

/// <summary>
/// �`��
/// </summary>
void GameScore::Render()
{
	// �X�R�A
	int thousandDigit = m_score / 1000;			 // 1000�̈�
	int hundredDigit = ((m_score / 100) % 10);   // 100�̈�
	int tenDigit = ((m_score % 100) / 10);		 // 10�̈�
	int oneDigit = m_score % 10;				 // 1�̈�

	// �X�R�A�w�i�̕`��
	mp_scoreBG->Render();

	// �X�R�A�̕`��
	for (int i = 0; i < Digit::NUM; i++)
	{
		// �\���ꏊ�ݒ�
		float charSpace = 10.0f;
		mp_scoreStr->SetPos(SimpleMath::Vector2(m_basePosX + (i*charSpace)+(i*mp_scoreStr->GetWidth()),
												mp_scoreStr->GetPos().y));

		RECT rect;
		float scoreWidth = mp_scoreStr->GetWidth();
		float scoreHeight = mp_scoreStr->GetHeight();
		// ���ɉ����Đ؂���ʒu��ς���
		switch (i)
		{
			// 1000�̈�
		case THOUSAND:
			rect = { LONG(scoreWidth * thousandDigit), LONG(0), LONG((scoreWidth * thousandDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// 100�̈�
		case HUNDRED:
			rect = { LONG(scoreWidth * hundredDigit), LONG(0), LONG((scoreWidth * hundredDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// 10�̈�
		case TEN:
			rect = { LONG(scoreWidth * tenDigit), 0, LONG((scoreWidth * tenDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// 1�̈�
		case ONE:
			rect = { LONG(scoreWidth * oneDigit), LONG(0), LONG((scoreWidth * oneDigit) + scoreWidth), LONG(scoreHeight) };
			mp_scoreStr->SetRect(float(rect.left), float(rect.top), float(rect.right), float(rect.bottom));
			break;
			// ����ȊO
		default:
			break;
		}
		mp_scoreStr->Render();
	}
}
