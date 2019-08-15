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
	  m_width(0.0f), m_height(0.0f),
	  m_alpha(0.0f), m_scale(0.0f),
	  m_isHover(false)
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
	RECT rect = { 0, 0, int(m_width), int(m_height) };
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	
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
	RECT rect = { 0, 0, int(m_width), int(m_height) };
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

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
	RECT rect = { 0, 0, int(m_width), int(m_height) };
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), m_scale, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), m_scale, SpriteEffects_None, 0);

	mp_sprite->End();
}

