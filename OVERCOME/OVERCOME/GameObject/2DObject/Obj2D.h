//////////////////////////////////////////////////////////////
// File.    Obj2D.h
// Summary. Obj2DClass,BaseClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SpriteBatch.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class Obj2D
{
// メンバー変数
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
	DirectX::SimpleMath::Vector2						m_pos;						// 位置
	float												m_width, m_height;			// 幅、高さ
	RECT												m_rect;						// 切り取り

	Color												m_color;					// 色情報
	float												m_alpha;					// スプライトのα値
	float												m_scale;					// スプライトのスケール値

	bool												m_isHover;					// ホバーしているかどうか

	std::unique_ptr<DirectX::SpriteBatch>				mp_sprite;					// スプライトバッチ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mp_texture;					// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	mp_textureHvr;				// テクスチャハンドル(ホバー時)

//private:

// メンバー関数
public:
	// コンストラクタ
	Obj2D();
	// デストラクタ
	~Obj2D();

	// 生成
	virtual void Create(const wchar_t* normalTexPath, const wchar_t* hoverTexPath);

	// 初期化
	virtual void Initialize(const DirectX::SimpleMath::Vector2& pos,
							const float width, const float height,
							const float alpha, const float scale);

	// 更新
	virtual bool Update(float elapsedTime);
	// 描画
	virtual void Render();
	virtual void RenderColor();
	virtual void RenderAlpha();
	virtual void RenderAlphaScale();

	// フェード
	bool Fade(float fadeValue, FADE fadeDir);

	// 衝突判定
	bool IsCollideMouse(const DirectX::SimpleMath::Vector2& mousePos,
						const DirectX::SimpleMath::Vector2& obj2DPos, const float obj2DWidth, const float obj2DHeight);

	//------------------------------------------Getter------------------------------------------//

	// 位置を取得
	DirectX::SimpleMath::Vector2 GetPos()		{ return m_pos; }
	// 幅を取得
	float GetWidth()							{ return m_width; }
	// 高さを取得
	float GetHeight()							{ return m_height; }
	// 色情報を取得
	Color GetColor()							{ return m_color; }
	// α値を取得
	float GetAlpha()							{ return m_alpha; }
	// スケール値を取得
	float GetScale()							{ return m_scale; }
	// ホバー状態を取得
	bool GetHover()								{ return m_isHover; }
	//------------------------------------------------------------------------------------------//

	//------------------------------------------Setter------------------------------------------//

	// 位置を設定
	void SetPos(const DirectX::SimpleMath::Vector2 position)		{ m_pos = position; }
	// 幅、高さを設定
	void SetWidthHeight(const float width, const float height)		{ m_width = width, m_height = height; }
	// 色情報を設定
	void SetColor(const float red, const float green, const float blue)
	{
		m_color.red = red;
		m_color.green = green;
		m_color.blue = blue;
	}
	// α値を設定
	void SetAlpha(const float alpha)								{ m_alpha = alpha; }
	// スケール値を設定
	void SetScale(const float scale)								{ m_scale = scale; }
	// ホバー状態を設定
	void SetHover(bool flag)										{ m_isHover = flag; }
	// 切り取りを設定
	void SetRect(const float left, const float top, const float right, const float bottom)
																	{ m_rect = { int(left), int(top), int(right), int(bottom) };}
	//------------------------------------------------------------------------------------------//

//private:

};