//////////////////////////////////////////////////////////////
// File.    GameDebug.cpp
// Summary. GameDebugClass
// Date.    2018/10/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "GameDebug.h"
#include "DeviceResources.h"

// usingディレクトリ
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/// <summary>
/// コンストラクタ
/// </summary>
GameDebug::GameDebug()
{
	// スプライトバッチの作成
	m_sprites = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// スプライトフォントの作成
	m_font = std::make_unique<SpriteFont>(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), L"SegoeUI_18.spritefont");
}
/// <summary>
/// デストラクタ
/// </summary>
GameDebug::~GameDebug()
{
}

/// <summary>
/// デバッグ描画
/// </summary>
/// <param name="debugLen">文字列デバッグ</param>
/// <param name="debugPos">表示位置</param>
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
/// intの値デバッグ
/// </summary>
/// <param name="debugLen">文字列デバッグ</param>
/// <param name="intValue">intの値</param>
/// <param name="debugPos">表示位置</param>
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
/// floatの値デバッグ
/// </summary>
/// <param name="debugLen">文字列デバッグ</param>
/// <param name="floatValue">floatの値</param>
/// <param name="debugPos">表示位置</param>
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
/// boolの値デバッグ
/// </summary>
/// <param name="debugLen">文字列デバッグ</param>
/// <param name="boolValue">boolの値</param>
/// <param name="debugPos">表示位置</param>
void GameDebug::DebugBoolRender(char * debugLen, bool boolValue, DirectX::SimpleMath::Vector2 debugPos)
{
	wstringstream ss;
	ss << debugLen << boolValue;
	wstring str = ss.str();

	m_sprites->Begin();
	m_font->DrawString(m_sprites.get(), str.c_str(), debugPos);
	m_sprites->End();
}
