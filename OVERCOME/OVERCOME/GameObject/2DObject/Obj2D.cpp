//////////////////////////////////////////////////////////////
// File.    Obj2D.cpp
// Summary. Obj2DClass,BaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "../../pch.h"
#include "Obj2D.h"

#include "../../Utility/CommonStateManager.h"

// usingディレクトリ
using namespace DirectX;


/// <summary>
/// コンストラクタ
/// </summary>
Obj2D::Obj2D()
	: m_pos(0.0f,0.0f),
	  m_width(0.0f), m_height(0.0f),
	  m_alpha(0.0f), m_scale(0.0f),
	  m_isHover(false)
{
}
/// <summary>
/// デストラクタ
/// </summary>
Obj2D::~Obj2D()
{
}

/// <summary>
/// 生成
/// </summary>
/// <param name="normalTexPath">スプライトのパス</param>
/// <param name="hoverTexPath">ホバー時スプライトのパス</param>
void Obj2D::Create(const wchar_t* normalTexPath, const wchar_t* hoverTexPath)
{
	// スプライトバッチの初期化
	mp_sprite = std::make_unique<SpriteBatch>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	// スプライトの読み込み
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), normalTexPath, nullptr, mp_texture.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), hoverTexPath, nullptr, mp_textureHvr.GetAddressOf());
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="pos">表示位置</param>
/// <param name="width">スプライトの幅</param>
/// <param name="height">スプライトの高さ</param>
/// <param name="alpha">スプライトのα値</param>
/// <param name="scale">スプライトのスケール値</param>
void Obj2D::Initialize(const DirectX::SimpleMath::Vector2 & pos, const float width, const float height, const float alpha, const float scale)
{
	m_pos = pos;

	// 幅、高さの設定
	m_width = width;
	m_height = height;

	// α値、スケール値の設定
	m_alpha = alpha;
	m_scale = scale;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <returns>終了状態</returns>
bool Obj2D::Update(float elapsedTime)
{
	return true;
}
/// <summary>
/// 描画
/// </summary>
void Obj2D::Render()
{
	// スプライトの描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// スプライトの表示
	RECT rect = { 0, 0, int(m_width), int(m_height) };
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	
	mp_sprite->End();
}
/// <summary>
/// 描画(α値変更有)
/// </summary>
void Obj2D::RenderAlpha()
{
	// スプライトの描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// スプライトの表示
	RECT rect = { 0, 0, int(m_width), int(m_height) };
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), 1.0f, SpriteEffects_None, 0);

	mp_sprite->End();
}
/// <summary>
/// 描画(α値スケール値変更有)
/// </summary>
void Obj2D::RenderAlphaScale()
{
	// スプライトの描画
	mp_sprite->Begin(SpriteSortMode_Deferred, CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied());

	// スプライトの表示
	RECT rect = { 0, 0, int(m_width), int(m_height) };
	if (mp_textureHvr != nullptr && m_isHover)
		mp_sprite->Draw(mp_textureHvr.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), m_scale, SpriteEffects_None, 0);
	else
		mp_sprite->Draw(mp_texture.Get(), m_pos, &rect, SimpleMath::Vector4(1.0f, 1.0f, 1.0f, m_alpha), 0.0f, XMFLOAT2(1.0f, 1.0f), m_scale, SpriteEffects_None, 0);

	mp_sprite->End();
}

