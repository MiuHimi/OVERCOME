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
	static int                                           m_score;                       // �v���C���[�����链�_
	bool                                                 m_deductOccurrence = false;    // �R�[�X�A�E�g���Ă��邩�𔻒�
	int                                                  m_compareColum;                // �v���C���[�W�����v�ɂ����_����p�ϐ�
	int                                                  m_compareLine;                 // �v���C���[�W�����v�ɂ����_����p�ϐ�
	bool                                                 m_getPointChance = false;      // �v���C���[�W�����v�ɂ����_����p�ϐ�

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBsckground;           // �e�N�X�`���n���h��
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

	// ���_�`�����X
	void AddPointChance(int j, int i);

	// Getter,Setter
	static int GetScore()                           { return m_score; }
	static void SetScore(int score)                 { m_score = score; }
	// ���_�̑���
	void FluctuationScore(int addscore)      { m_score += addscore; }
	// �v���C���[���R�[�X�A�E�g������t���O������
	void SetDeductFlag(bool flag)            { m_deductOccurrence = flag; }
	// �v���C���[�W�����v�ɂ����_����p�̒l�ݒ�
	void SetAddPointChance(int j, int i)     { m_compareColum = j; m_compareLine = i;}
	// �v���C���[���_�l���`�����X������
	bool GetPointChance()                    { return m_getPointChance; }
	// �v���C���[���_�l���`�����X�ݒ�
	void SetPointChance(bool flag)           { m_getPointChance = flag; }

private:

};