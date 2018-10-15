//////////////////////////////////////////////////////////////
// File.    GameTimer.h
// Summary. GameTimerClass
// Date.    2018/10/01
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <queue>

#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

class GameTimer
{
// �����o�[�ϐ�
public:

private:
	int                                                  m_remainingTime;               // �c�莞��

	float                                                m_timeNumWidth;                // �����̕�
	float                                                m_timeNumHeight;               // �����̍���

	bool                                                 m_timeUpFlag;                  // �^�C���A�b�v�ɂȂ��Ă��邩�ǂ���

	std::queue<int>                                      m_queueDigit;                  // ���Ԑݒ�p
	
	enum CountDigit
	{
		e_minuteUpperDigit,
		e_minuteLowerDigit,
		e_secondUpperDigit,
		e_secondLowerDigit,

		e_numDigit
	};
	DirectX::SimpleMath::Vector2                         m_posBackground;               // �^�C�}�[�w�i�ʒu
	DirectX::SimpleMath::Vector2                         m_posCountDigit[e_numDigit];   // �^�C�}�[����ʒu

	std::unique_ptr<DirectX::SpriteBatch>                m_sprites;                     // �X�v���C�g�o�b�`
	std::unique_ptr<DirectX::CommonStates>               m_states;                      // �R�����X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBackground;           // �e�N�X�`���n���h��(�w�i)
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureNum;                  // �e�N�X�`���n���h��(����)

// �����o�[�֐�
public:
	// �R���X�g���N�^
	GameTimer();
	// �f�X�g���N�^
	~GameTimer();

	// ����
	void Create();
	// �X�V
	bool Update(DX::StepTimer const& timer);
	// �`��
	void Render();

	// Getter,Setter
	// ���Ԏ擾�A���Ԑݒ�
	int GetTime()               { return m_remainingTime; }
	void SetTime(int time)      { m_remainingTime = time; }
	// �������ԓ�(false)or�������ԓ��B(true)
	bool GetTimeUpFlag()        { return m_timeUpFlag; }

private:

};
