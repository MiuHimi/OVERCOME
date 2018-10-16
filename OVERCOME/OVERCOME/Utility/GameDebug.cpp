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
/// �f�o�b�O�`��
/// </summary>
/// <param name="debugLen">������f�o�b�O</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss;
	ss << debugLen;
	wstring str = ss.str();

	m_sprites->Begin();
	m_font->DrawString(m_sprites.get(), str.c_str(), debugPos);
	m_sprites->End();
}
/// <summary>
/// int�̒l�f�o�b�O
/// </summary>
/// <param name="debugLen">������f�o�b�O</param>
/// <param name="intValue">int�̒l</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugIntRender(char * debugLen, int intValue, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss;
	ss << debugLen << intValue;
	wstring str = ss.str();

	m_sprites->Begin();
	m_font->DrawString(m_sprites.get(), str.c_str(), debugPos);
	m_sprites->End();
}
/// <summary>
/// float�̒l�f�o�b�O
/// </summary>
/// <param name="debugLen">������f�o�b�O</param>
/// <param name="floatValue">float�̒l</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugFloatRender(char * debugLen, float floatValue, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss;
	ss << debugLen << floatValue;
	wstring str = ss.str();

	m_sprites->Begin();
	m_font->DrawString(m_sprites.get(), str.c_str(), debugPos);
	m_sprites->End();
}
/// <summary>
/// bool�̒l�f�o�b�O
/// </summary>
/// <param name="debugLen">������f�o�b�O</param>
/// <param name="boolValue">bool�̒l</param>
/// <param name="debugPos">�\���ʒu</param>
void GameDebug::DebugBoolRender(char * debugLen, bool boolValue, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss;
	ss << debugLen << boolValue;
	wstring str = ss.str();

	m_sprites->Begin();
	m_font->DrawString(m_sprites.get(), str.c_str(), debugPos);
	m_sprites->End();
}
