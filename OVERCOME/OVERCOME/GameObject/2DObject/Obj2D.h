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
public:
	struct Color
	{
		float red;
		float green;
		float blue;
	};

	enum FADE
	{
		NONE,
		FADE_IN,
		FADE_OUT
	};

private:
	DirectX::SimpleMath::Vector2						m_pos;						// �ʒu
	float												m_width, m_height;			// ���A����
	RECT												m_rect;						// �؂���

	Color												m_color;					// �F���
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
	virtual void RenderColor();
	virtual void RenderAlpha();
	virtual void RenderAlphaScale();

	// �t�F�[�h
	bool Fade(float fadeValue, FADE fadeDir);

	// �Փ˔���
	bool IsCollideMouse(const DirectX::SimpleMath::Vector2& mousePos,
						const DirectX::SimpleMath::Vector2& obj2DPos, const float obj2DWidth, const float obj2DHeight);

	//------------------------------------------Getter------------------------------------------//

	// �ʒu���擾
	DirectX::SimpleMath::Vector2 GetPos()		{ return m_pos; }
	// �����擾
	float GetWidth()							{ return m_width; }
	// �������擾
	float GetHeight()							{ return m_height; }
	// �F�����擾
	Color GetColor()							{ return m_color; }
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
	// �F����ݒ�
	void SetColor(const float red, const float green, const float blue)
	{
		m_color.red = red;
		m_color.green = green;
		m_color.blue = blue;
	}
	// ���l��ݒ�
	void SetAlpha(const float alpha)								{ m_alpha = alpha; }
	// �X�P�[���l��ݒ�
	void SetScale(const float scale)								{ m_scale = scale; }
	// �z�o�[��Ԃ�ݒ�
	void SetHover(bool flag)										{ m_isHover = flag; }
	// �؂����ݒ�
	void SetRect(const float left, const float top, const float right, const float bottom)
																	{ m_rect = { int(left), int(top), int(right), int(bottom) };}
	//------------------------------------------------------------------------------------------//

//private:

};