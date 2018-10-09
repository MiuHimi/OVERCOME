//////////////////////////////////////////////////////////////
// File.    GameTimer.cpp
// Summary. GameTimerClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "GameTimer.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameTimer::GameTimer()
	: m_remainingTime(180)
	, m_timeUpFlag(false)
	, m_posBackground(DirectX::SimpleMath::Vector2(20.0f, 20.0f))
{
	for (int i = 0; i < e_numDigit; ++i)
	{
		m_posCountDigit[i] = DirectX::SimpleMath::Vector2(m_posBackground);
	}

	// �X�v���C�g�o�b�`�̍쐬
	m_sprites = std::make_unique<DirectX::SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());
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
	
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_0_image.png", nullptr, m_textureNum[0].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_1_image.png", nullptr, m_textureNum[1].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_2_image.png", nullptr, m_textureNum[2].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_3_image.png", nullptr, m_textureNum[3].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_4_image.png", nullptr, m_textureNum[4].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_5_image.png", nullptr, m_textureNum[5].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_6_image.png", nullptr, m_textureNum[6].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_7_image.png", nullptr, m_textureNum[7].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_8_image.png", nullptr, m_textureNum[8].GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\GameTimer\\timer_9_image.png", nullptr, m_textureNum[9].GetAddressOf());

	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"Resources\\Images\\himi_logo.png", nullptr, m_textureTest.GetAddressOf());
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
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());

	// �������Ԕw�i�̕`��
	m_sprites->Draw(m_textureBackground.Get(), m_posBackground);
	// �������Ԑ���̕`��
	for (int i = 0; i < e_numDigit; ++i)
	{
		int num = m_queueDigit.front();
		m_sprites->Draw(m_textureNum[num].Get(), m_posCountDigit[i]);
		m_queueDigit.pop();
		/*RECT rect;
		rect.bottom = 160.0f;
		rect.left = 0.0f;
		rect.right = 160.0f;
		rect.top = 0.0f;
		m_sprites->Draw(m_textureTest.Get(), DirectX::SimpleMath::Vector2(300.0f, 400.0f), &rect, DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f),1, DirectX::SpriteEffects_None, 0.0f);*/
	}

	m_sprites->End();
}
