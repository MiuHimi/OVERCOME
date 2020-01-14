//////////////////////////////////////////////////////////////
// File.    DrawableList.h
// Summary. DrawableListClass
// Date.    2019/12/18
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <map>
#include <memory>
#include <algorithm>
#include "Drawable.h"

class DrawableList
{
// メンバー変数
public:
	enum DRAW_LAYER
	{
		STATIC_OBJ_3D,
		DYNAMIC_OBJ_3D,
		STATIC_OBJ_2D,
		DYNAMIC_OBJ_2D,
		UI
	};

private:
	std::multimap<int, std::shared_ptr<Drawable>> m_drawables;	// リスト

// メンバー関数
public:
	// コンストラクタ
	DrawableList() = default;
	// デストラクタ
	~DrawableList() = default;

	// 追加
	void Add(std::shared_ptr<Drawable> drawable, int layer)
	{
		// リストに追加する
		m_drawables.insert(std::make_pair(layer, drawable));
	}

	// 描画
	void Draw() const
	{
		// リストに追加されているオブジェクトを順に見ていき、
		std::for_each(m_drawables.begin(), m_drawables.end(), [](auto p)
		{
			// 表示できるオブジェクトがあれば
			if (p.second->IsVisible())
			{
				// 描画する
				p.second->Draw();
			}
		});
	}
};