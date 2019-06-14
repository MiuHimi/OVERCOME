//////////////////////////////////////////////////////////////
// File.    GameScore.cpp
// Summary. GameScoreClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "../2DObject/GameScore.h"

#include "../../Utility/CommonStateManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScore::GameScore()
	: m_scorePos(0.0f, 0.0f),
	  m_scoreBGPos(0.0f, 0.0f),
	  m_scoreWidth(0.0f), m_scoreHeight(0.0f),
	  m_scoreBGWidth(0.0f), m_scoreBGHeight(0.0f),
	  mp_texture(nullptr),
	  mp_textureBackground(nullptr),
	  mp_sprite(nullptr)
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
void GameScore::Create(const wchar_t* scoreFileName, const wchar_t* backgroundFileName)
{
	// �e�ϐ��̏�����
	m_score = 0;

	m_scorePos = SimpleMath::Vector2(650.0f, 560.0f);
	m_scoreBGPos = SimpleMath::Vector2(700.0f, 560.0f);

	m_scoreWidth = 40.0f; 
	m_scoreHeight = 60.0f;
	m_scoreBGWidth = 200.0f;
	m_scoreBGHeight = 80.0f;

	// �e�N�X�`���̃��[�h
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), scoreFileName, nullptr, mp_texture.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), backgroundFileName, nullptr, mp_textureBackground.GetAddressOf());
	
	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

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
	int hundredDigit = m_score / 100;		// 100�̈�
	int tenDigit = ((m_score % 100) / 10);  // 10�̈�
	int oneDigit = m_score % 10;			// 1�̈�

	// �X�R�A�̕`��
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// �w�i
	RECT rectBG = { 0, 0, 200, 80 };
	mp_sprite->Draw(mp_textureBackground.Get(), SimpleMath::Vector2(m_scoreBGPos.x - m_scoreBGWidth / 2.0f, m_scoreBGPos.y - m_scoreBGHeight / 2.0f), &rectBG, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	// �X�R�A
	for (int i = 0; i < Digit::NUM; i++)
	{
		RECT rect;
		// ���ɉ����Đ؂���ʒu��ς���
		switch (i)
		{
		// 100�̈�
		case HUNDRED:
			rect = { int(m_scoreWidth) * hundredDigit, 0, int(m_scoreWidth) * hundredDigit + int(m_scoreWidth), int(m_scoreHeight) };
			break;
		// 10�̈�
		case TEN:
			rect = { int(m_scoreWidth) * tenDigit, 0, int(m_scoreWidth) * tenDigit + int(m_scoreWidth), int(m_scoreHeight) };
			break;
		// 1�̈�
		case ONE:
			rect = { int(m_scoreWidth) * oneDigit, 0, int(m_scoreWidth) * oneDigit + int(m_scoreWidth), int(m_scoreHeight) };
			break;
		// ����ȊO�̈�
		default:
			break;
		}

		float indent = 0;
		if (i == 0)
		{
			indent = 0;
		}
		else
		{
			indent = 10;
		}
		mp_sprite->Draw(mp_texture.Get(), SimpleMath::Vector2(m_scorePos.x + float(i * m_scoreWidth) + indent, m_scorePos.y - (float)m_scoreHeight / 2.0f), &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	}

	mp_sprite->End();
}
