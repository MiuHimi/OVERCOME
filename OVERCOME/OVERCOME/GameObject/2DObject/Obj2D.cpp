//////////////////////////////////////////////////////////////
// File.    Obj2D.cpp
// Summary. Obj2DClass,BaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "Obj2D.h"

#include "../../Utility/CommonStateManager.h"

// using�f�B���N�g��
using namespace DirectX;


/// <summary>
/// �R���X�g���N�^
/// </summary>
Obj2D::Obj2D()
	: m_pos(0.0f,0.0f),
	  m_width(0.0f), m_height(0.0f), m_rect{0,0,0,0},
	  m_alpha(0.0f), m_scale(0.0f),
	  m_isHover(false),
	  mp_sprite(nullptr),
	  mp_texture(nullptr),mp_textureHvr(nullptr)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Obj2D::~Obj2D()
{
}

/// <summary>
/// ����
/// </summary>
/// <param name="normalTexPath">�X�v���C�g�̃p�X</param>
/// <param name="hoverTexPath">�z�o�[���X�v���C�g�̃p�X</param>
void Obj2D::Create(const wchar_t* normalTexPath, const wchar_t* hoverTexPath)
{
	// �X�v���C�g�o�b�`�̏�����
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// �X�v���C�g�̓ǂݍ���
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), normalTexPath, nullptr, mp_texture.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), hoverTexPath, nullptr, mp_textureHvr.GetAddressOf());
}

/// <summary>
/// ������
/// </summary>
/// <param name="pos">�\���ʒu</param>
/// <param name="width">�X�v���C�g�̕�</param>
/// <param name="height">�X�v���C�g�̍���</param>
/// <param name="alpha">�X�v���C�g�̃��l</param>
/// <param name="scale">�X�v���C�g�̃X�P�[���l</param>
void Obj2D::Initialize(const DirectX::SimpleMath::Vector2 & pos, const float width, const float height, const float alpha, const float scale)
{
	m_pos = pos;

	// ���A�����̐ݒ�
	m_width = width;
	m_height = height;

	// ���l�A�X�P�[���l�̐ݒ�
	m_alpha = alpha;
	m_scale = scale;
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <returns>�I�����</returns>
bool Obj2D::Update(float elapsedTime)
{
	return true;
}
/// <summary>
/// �`��
/// </summary>
void Obj2D::Render()
{
	// �X�v���C�g�̕`��
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// �X�v���C�g�̕\��
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &m_rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &m_rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	
	mp_sprite->End();
}
/// <summary>
/// �`��(���l�ύX�L)
/// </summary>
void Obj2D::RenderAlpha()
{
	// �X�v���C�g�̕`��
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// �X�v���C�g�̕\��
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &m_rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &m_rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	mp_sprite->End();
}
/// <summary>
/// �`��(���l�X�P�[���l�ύX�L)
/// </summary>
void Obj2D::RenderAlphaScale()
{
	// �X�v���C�g�̕`��
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// �X�v���C�g�̕\��
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &m_rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), m_scale, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &m_rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), m_scale, SpriteEffects_None, 0);

	mp_sprite->End();
}

/// <summary>
/// �t�F�[�h����
/// </summary>
/// <param name="fadeValue">�t�F�[�h����傫��</param>
/// <param name="fadeDir">FADE_IN=�t�F�[�h�C���AFADE_OUT=�t�F�[�h�A�E�g</param>
/// <returns>true=�t�F�[�h�����Afalse=�t�F�[�h��</returns>
bool Obj2D::Fade(float fadeValue, FADE fadeDir)
{
	switch (fadeDir)
	{
	case Obj2D::NONE:
		break;
	case Obj2D::FADE_IN:
		if (m_alpha != 0.0f) m_alpha -= fadeValue;
		if (m_alpha <= 0.0f) m_alpha = 0.0f;
		break;
	case Obj2D::FADE_OUT:
		if (m_alpha != 1.0f) m_alpha += fadeValue;
		if (m_alpha >= 1.0f) m_alpha = 1.0f;
		break;
	default:
		break;
	}

	if (m_alpha != 0.0f && m_alpha != 1.0f)return false;
	else return true;
}

/// <summary>
/// �}�E�X�Ƃ̏Փ˔���
/// </summary>
/// <param name="objPos">�}�E�X�̈ʒu</param>
/// <param name="obj2DPos">2D�I�u�W�F�N�g�̈ʒu</param>
/// <param name="obj2DWidth">2D�I�u�W�F�N�g�̕�</param>
/// <param name="obj2DHeight">2D�I�u�W�F�N�g�̍���</param>
/// <returns></returns>
bool Obj2D::IsCollideMouse(const DirectX::SimpleMath::Vector2& mousePos, 
						   const DirectX::SimpleMath::Vector2& obj2DPos, const float obj2DWidth, const float obj2DHeight)
{
	if (obj2DPos.x < mousePos.x && mousePos.x < (obj2DPos.x + obj2DWidth) &&
		obj2DPos.y < mousePos.y && mousePos.y < (obj2DPos.y + obj2DHeight))
	{
		return true;
	}

	return false;
}
