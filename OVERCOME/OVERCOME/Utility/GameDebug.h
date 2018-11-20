//////////////////////////////////////////////////////////////
// File.    GameDebug.h
// Summary. GameDebugClass
// Date.    2018/11/17
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include "../pch.h"
#include "SingletonBase.h"

// デバッグ
#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

class GameDebug : public SingletonBase<GameDebug>
{
// メンバー変数
public:

private:
	std::unique_ptr<DirectX::SpriteBatch>   m_sprites;       // スプライトバッチ
	std::unique_ptr<DirectX::SpriteFont>    m_font;          // スプライトフォント

	struct DebugTextData                                     // テキスト情報の構造体
	{
		std::wstring text;
		DirectX::SimpleMath::Vector2 pos;
		DebugTextData(std::wstring text, DirectX::SimpleMath::Vector2 pos)
		{
			this->text = text;
			this->pos = pos;
		}
	};

	std::list<DebugTextData> m_debugText;					 // デバッグで表示するテキストリスト

// メンバー関数
public:
	~GameDebug();
	void Render();

	// フレンド関数
	friend SingletonBase<GameDebug>;

	// デバッグ描画
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(float debugLen, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(DirectX::SimpleMath::Vector2 debugInf, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(char* debugLen, float debugInf, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugInf, DirectX::SimpleMath::Vector2 debugPos);
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector3 debugInf, DirectX::SimpleMath::Vector2 debugPos);

private:
	GameDebug();

	//-型変換関数-------------------------------------

	std::wstring NumtoWstring(float num);
	std::wstring ChartoWstring(char* text);
	//------------------------------------------------
};
