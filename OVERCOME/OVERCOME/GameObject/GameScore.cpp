//////////////////////////////////////////////////////////////
// File.    GameScore.cpp
// Summary. GameScoreClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "GameScore.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
GameScore::GameScore()
	                 : m_score(100)
	                 , m_timeCount(0)
	                 , m_deductTimeCount(0)
	                 , m_compareColum(0)
	                 , m_compareLine(0)
{	  
    // �X�v���C�g�o�b�`�̍쐬
	m_sprites = std::make_unique<DirectX::SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());
	// �R�����X�e�[�g�̍쐬 
	m_states = std::make_unique<DirectX::CommonStates>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice());
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
void GameScore::Create()
{
	// �e�N�X�`���̃��[�h
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
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�N���o�ߎ���</param>
bool GameScore::Update(DX::StepTimer const& timer)
{
	// ���Ԍo�߂ɂ�錸�_
	m_timeCount++;
	if (m_timeCount > 180)
	{
		m_timeCount = 0;
		GameScore::FluctuationScore(-10);
	}
	
	// �R�[�X�A�E�g�ɂ�錸�_
	if (m_deductOccurrence == true)
	{
		m_deductTimeCount++;
		if (m_deductTimeCount > 60)
		{
			GameScore::FluctuationScore(-5);
			m_deductTimeCount = 0;
		}
	}
	if (m_deductOccurrence == false)
	{
		m_deductTimeCount = 0;
	}

	// �X�R�A�̉���
	if (GameScore::GetScore() <= 0)
	{
		GameScore::SetScore(0);
	}
	
	return true;
}

/// <summary>
/// �`��
/// </summary>
void GameScore::Render()
{
	int hundredDigit = m_score / 100;
	int tenDigit = ((m_score % 100) / 10);
	int oneDigit = m_score % 10;

	// �X�v���C�g�̕`��
	m_sprites->Begin(DirectX::SpriteSortMode_Deferred, m_states->NonPremultiplied());

	m_sprites->Draw(m_textureNum[hundredDigit].Get(), DirectX::SimpleMath::Vector2(650.0f, 530.0f));
	m_sprites->Draw(m_textureNum[tenDigit].Get()    , DirectX::SimpleMath::Vector2(700.0f, 530.0f));
	m_sprites->Draw(m_textureNum[oneDigit].Get()    , DirectX::SimpleMath::Vector2(750.0f, 530.0f));

	m_sprites->End();
}

/// <summary>
/// �W�����v�ŏ��z����ꂽ����_
/// </summary>
/// <param name="j">�s���W</param>
/// <param name="i">����W</param>
void GameScore::AddPointChance(int j, int i)
{
	// �ȑO�ɂ������W�ƈႦ��
	if (j != m_compareColum || i != m_compareLine)
	{
		// ���_
		GameScore::FluctuationScore(10);
		// ���W�Đݒ�
		GameScore::SetAddPointChance(j, i);
	}
}
