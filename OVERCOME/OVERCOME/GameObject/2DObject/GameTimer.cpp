//////////////////////////////////////////////////////////////
// File.    GameTimer.cpp
// Summary. GameTimerClass
// Date.    2018/10/16
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "GameTimer.h"

#include "../../Utility/DrawManager.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameTimer::GameTimer()
	: m_remainingTime(180)
	, m_timeNumWidth(40)
	, m_timeNumHeight(60)
	, m_timeUpFlag(false)
	, m_posBackground(DirectX::SimpleMath::Vector2(20.0f, 20.0f))
{
	// �����̏ꏊ���w�i�Ɠ����Ƃ���ŏ��������Ă���
	for (int i = 0; i < e_numDigit; ++i)
	{
		m_posCountDigit[i] = DirectX::SimpleMath::Vector2(m_posBackground);
	}

	// �R�����X�e�[�g�̍쐬 
	m_states = std::make_unique<DirectX::CommonStates>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameTimer::~GameTimer()
{
}

/// <summary>
/// ����
/// </summary>
void GameTimer::Create()
{
	// �e�N�X�`���̃��[�h
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_background_image.png", nullptr, m_textureBackground.GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_len_image.png", nullptr, m_textureNum.GetAddressOf());
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
bool GameTimer::Update(DX::StepTimer const& timer)
{
	static int countTime = 0;
	countTime++;

	// 60�t���[���łP�b�o��
	if (countTime > 60)
	{
		m_remainingTime--;
		countTime = 0;
	}

	// �����Ƃ̐����̐ݒ�
	m_queueDigit.push((m_remainingTime / 60) / 10);
	m_queueDigit.push((m_remainingTime / 60) % 10);
	m_queueDigit.push((m_remainingTime % 60) / 10);
	m_queueDigit.push((m_remainingTime % 60) % 10);

	// �������Ԃ�0�ɂȂ�����t���O�𗧂Ă�
	if (m_remainingTime == 0)
	{
		m_timeUpFlag = true;
	}

	return true;
}

/// <summary>
/// �`��
/// </summary>
void GameTimer::Render()
{
	// �����Ƃ̈ʒu�ݒ�
	m_posCountDigit[e_minuteUpperDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 5, m_posBackground.y + 8);
	m_posCountDigit[e_minuteLowerDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 50, m_posBackground.y + 8);
	m_posCountDigit[e_secondUpperDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 110, m_posBackground.y + 8);
	m_posCountDigit[e_secondLowerDigit] = DirectX::SimpleMath::Vector2(m_posBackground.x + 155, m_posBackground.y + 8);

	// �X�v���C�g�̕`��
	// �������Ԕw�i�̕`��
	DrawManager::SingletonGetInstance().Draw(m_textureBackground.Get(), m_posBackground);
	// �������Ԑ���̕`��
	for (int i = 0; i < e_numDigit; ++i)
	{
		// �؂��鐔����ݒ�
		float u = m_timeNumWidth * m_queueDigit.front();
		m_queueDigit.pop();
		// �؂���ꏊ��ݒ�
		RECT rect;
		rect.top    = LONG(0.0f);
		rect.left   = LONG(u);
		rect.right  = LONG(u + m_timeNumWidth);
		rect.bottom = LONG(m_timeNumHeight);
		
		//DrawManager::SingletonGetInstance().DrawRect(m_textureNum.Get(), m_posCountDigit[i], &rect);
	}

}
