//////////////////////////////////////////////////////////////
// File.    Obj2D.h
// Summary. Obj2DClass,BaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SpriteBatch.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class Obj2D
{
// �����o�[�ϐ�
//public:

private:
	DirectX::SimpleMath::Vector2						m_pos;						// �ʒu
	float												m_width, m_height;			// ���A����

	float												m_alpha;					// �X�v���C�g�̃��l
	float												m_scale;					// �X�v���C�g�̃X�P�[���l

	bool												m_isHover;					// �z�o�[���Ă��邩�ǂ���

	std::unique_ptr<DirectX::SpriteBatch>				mp_sprite;					// �X�v���C�g�o�b�`
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mp_texture;					// �e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mp_textureHvr;				// �e�N�X�`���n���h��(�z�o�[��)

//private:

// �����o�[�֐�
public:
	// �R���X�g���N�^
	Obj2D();
	// �f�X�g���N�^
	~Obj2D();

	// ����
	virtual void Create(const wchar_t* normalTexPath, const wchar_t* hoverTexPath);

	// ������
	virtual void Initialize(const DirectX::SimpleMath::Vector2& pos,
							const float width, const float height,
							const float alpha, const float scale);

	// �X�V
	virtual bool Update(float elapsedTime);
	// �`��
	virtual void Render();
	virtual void RenderAlpha();
	virtual void RenderAlphaScale();

	//------------------------------------------Getter------------------------------------------//

	// �ʒu���擾
	DirectX::SimpleMath::Vector2 GetPos()		{ return m_pos; }
	// �����擾
	float GetWidth()							{ return m_width; }
	// �������擾
	float GetHeight()							{ return m_height; }
	// ���l���擾
	float GetAlpha()							{ return m_alpha; }
	// �X�P�[���l���擾
	float GetScale()							{ return m_scale; }
	// �z�o�[��Ԃ��擾
	bool GetHover()								{ return m_isHover; }
	//------------------------------------------------------------------------------------------//

	//------------------------------------------Setter------------------------------------------//

	// �ʒu��ݒ�
	void SetPos(const DirectX::SimpleMath::Vector2 position)		{ m_pos = position; }
	// ���A������ݒ�
	void SetWidthHeight(const float width, const float height)		{ m_width = width, m_height = height; }
	// ���l�A�X�P�[���l��ݒ�
	void SetAlphaScale(const float alpha, const float scale)		{ m_alpha = alpha, m_scale = scale; }
	// �z�o�[��Ԃ�ݒ�
	void SetHover(bool flag)										{ m_isHover = flag; }
	//------------------------------------------------------------------------------------------//

};