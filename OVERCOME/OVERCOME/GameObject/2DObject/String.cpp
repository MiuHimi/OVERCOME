//////////////////////////////////////////////////////////////
// File.    String.cpp
// Summary. StringClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "String.h"

#include "../../Utility/DrawManager.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;


/// <summary>
/// �R���X�g���N�^
/// </summary>
String::String()
	: m_texture(nullptr), m_pos(0.0f, 0.0f), m_color(Colors::White)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
/// <param name="game"></param>
/// <param name="texture"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="str"></param>
/// <param name="color"></param>
void String::Initialize(ID3D11ShaderResourceView * texture, DirectX::SimpleMath::Vector2 pos, const char* str, FXMVECTOR color)
{
	m_texture = texture;
	m_pos.x = pos.x;
	m_pos.y = pos.y;
	SetStr(str);
	m_color = color;
}

/// <summary>
/// �`��
/// </summary>
void String::Draw()
{
	float posX = m_pos.x;
	float posY = m_pos.y;

	if (!m_texture) return;

	for (size_t i = 0; i < m_str.size(); i++)
	{
		if (m_str[i] >= ' ' && m_str[i] <= 'z')
		{
			int str = m_str[i] - ' ';
			int wid = str % 16;
			int hei = str / 16;
			RECT rect = { wid * SIZE, hei * SIZE, (wid + 1) * SIZE, (hei + 1) * SIZE };
			//DrawManager::SingletonGetInstance().Draw(m_texture, Vector2(posX, posY), &rect, m_color);
			DrawManager::SingletonGetInstance().DrawRect(m_texture, Vector2(posX, posY), &rect);
		}
		posX += SIZE;
	}
}

/// <summary>
/// �������ݒ�
/// </summary>
/// <param name="str"></param>
void String::SetStr(const char * str)
{
	m_str = std::string(str);
}
/// <summary>
/// �ʒu��ݒu
/// </summary>
/// <param name="x">X���W</param>
/// <param name="y">Y���W</param>
void String::SetPos(DirectX::SimpleMath::Vector2 pos)
{
	m_pos.x = pos.x;
	m_pos.y = pos.y;
}
