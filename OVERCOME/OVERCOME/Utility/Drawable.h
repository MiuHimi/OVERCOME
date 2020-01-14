//////////////////////////////////////////////////////////////
// File.    Drawable.h
// Summary. DrawableClass
// Date.    2019/12/18
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

class Drawable
{
// メンバー変数
private:
	bool m_isVisible;		// 表示状態フラグ

// メンバー関数
public:
	// コンストラクタ
	Drawable() : m_isVisible(true) {};
	// デストラクタ
	virtual ~Drawable() = default;

	// 描画関数
	virtual void Draw() const = 0;

	// 表示状態設定
	void SetVisible(bool visibleFlag)	{ m_isVisible = visibleFlag; }

	// 表示状態取得
	bool IsVisible() const				{ return m_isVisible; }
};