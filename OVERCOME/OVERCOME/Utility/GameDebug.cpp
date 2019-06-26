//////////////////////////////////////////////////////////////
// File.    GameDebug.cpp
// Summary. GameDebugClass
// Date.    2018/10/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "GameDebug.h"
#include "DeviceResources.h"

// using�f�B���N�g��
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/// <summary>
/// �R���X�g���N�^
/// </summary>
GameDebug::GameDebug()
{
	// �X�v���C�g�o�b�`�̍쐬
	m_sprites = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// �X�v���C�g�t�H���g�̍쐬
	m_font = std::make_unique<SpriteFont>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"SegoeUI_18.spritefont");
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
GameDebug::~GameDebug()
{
}

/// <summary>
/// float�^����wstring�^�ւ̕ϊ�
/// </summary>
/// <param name="text">�ϊ����鐔�l</param>
/// <returns>�ϊ�����wstring������</returns>
wstring GameDebug::NumtoWstring(float num)
{
	wstringstream ss;
	ss << "" << num;
	wstring str = ss.str();

	return str;
}

/// <summary>
/// char�^����wstring�^�ւ̕ϊ�
/// </summary>
/// <param name="text">�ϊ����镶��</param>
/// <returns>�ϊ�����wstring������</returns>
std::wstring GameDebug::ChartoWstring(char * text)
{
	wstringstream ss;
	ss << "" << text;
	wstring str = ss.str();

	return str;
}

/// <summary>
/// �f�o�b�O�`��
/// </summary>
void GameDebug::Render()
{
	// ���X�g�̒��ɂ���v�f�̐�
	int listdata = 0;

	//�X�v���C�g�̕\���J�n
	m_sprites->Begin();

	//���X�g�̒��ɂ�����̂�\��
	for (std::list<DebugTextData>::iterator it = m_debugText.begin(); it != m_debugText.end(); it++)
	{
		m_font->DrawString(m_sprites.get(), it->text.c_str(), it->pos);
	}

	//�X�v���C�g�̕\���I��
	m_sprites->End();

	//���X�g�̒��ɂ�����𐔂���
	for (std::list<DebugTextData>::iterator it = m_debugText.begin(); it != m_debugText.end(); it++)
	{
		listdata++;
	}

	//���X�g�̒�������o��
	for (int i = 0; i < listdata; i++)
	{
		m_debugText.pop_back();
	}

}

/// <summary>
/// �f�o�b�O�`��
/// </summary>
/// <param name="debugLen">������f�o�b�O</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugPos)
{
	m_debugText.push_back(DebugTextData(ChartoWstring(debugLen), debugPos));
}

/// <summary>
/// ���X�g�Ƀf�[�^��ǉ�
/// </summary>
/// <param name="debugLen">�f�o�b�O���l(float)</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(float debugLen, DirectX::SimpleMath::Vector2 debugPos)
{
	m_debugText.push_back(DebugTextData(NumtoWstring(debugLen), debugPos));
}

/// <summary>
/// ���X�g�Ƀf�[�^��ǉ�����
/// </summary>
/// <param name="debugInf">�f�o�b�O���l(Vector2)</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(DirectX::SimpleMath::Vector2 debugInf, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ssX, ssY;
	ssX << "X = ";
	ssY << " , Y = ";
	wstring str = ssX.str() + NumtoWstring(debugInf.x) + ssY.str() + NumtoWstring(debugInf.y);

	m_debugText.push_back(DebugTextData(str, debugPos));
}

/// <summary>
/// ���X�g�Ƀf�[�^��ǉ�����
/// </summary>
/// <param name="debugLen">�f�o�b�O���閼�O</param>
/// <param name="debugInf">�f�o�b�O���l(float)</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(char * debugLen, float debugInf, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss;
	ss << debugLen << " : ";
	wstring str = ss.str() + NumtoWstring(debugInf);

	m_debugText.push_back(DebugTextData(str, debugPos));
}

/// <summary>
/// ���X�g�Ƀf�[�^��ǉ�����
/// </summary>
/// <param name="debugLen">�f�o�b�O���閼�O</param>
/// <param name="debugInf">�f�o�b�O���l(Vector2)</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(char * debugLen, DirectX::SimpleMath::Vector2 debugInf, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss, ssY;
	ss << debugLen << " : X = ";
	ssY << " , Y = ";
	wstring str = ss.str() + NumtoWstring(debugInf.x) + ssY.str() + NumtoWstring(debugInf.y);

	m_debugText.push_back(DebugTextData(str, debugPos));
}
/// <summary>
/// ���X�g�Ƀf�[�^��ǉ�����
/// </summary>
/// <param name="debugLen">�f�o�b�O���閼�O</param>
/// <param name="debugInf">�f�o�b�O���l(Vector3)</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(char * debugLen, DirectX::SimpleMath::Vector3 debugInf, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss, ssY, ssZ;
	ss << debugLen << " : X = ";
	ssY << " , Y = ";
	ssZ << " , Z = ";
	wstring str = ss.str() + NumtoWstring(debugInf.x) + ssY.str() + NumtoWstring(debugInf.y) + ssZ.str() + NumtoWstring(debugInf.z);

	m_debugText.push_back(DebugTextData(str, debugPos));
}