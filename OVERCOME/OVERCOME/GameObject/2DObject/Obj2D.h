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
//public:

private:
	DirectX::SimpleMath::Vector2						m_pos;						// 位置
	float												m_width, m_height;			// 幅、高さ

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
	virtual void RenderAlpha();
	virtual void RenderAlphaScale();

	//------------------------------------------Getter------------------------------------------//

	// 位置を取得
	DirectX::SimpleMath::Vector2 GetPos()		{ return m_pos; }
	// 幅を取得
	float GetWidth()							{ return m_width; }
	// 高さを取得
	float GetHeight()							{ return m_height; }
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
	// α値、スケール値を設定
	void SetAlphaScale(const float alpha, const float scale)		{ m_alpha = alpha, m_scale = scale; }
	// ホバー状態を設定
	void SetHover(bool flag)										{ m_isHover = flag; }
	//------------------------------------------------------------------------------------------//

};