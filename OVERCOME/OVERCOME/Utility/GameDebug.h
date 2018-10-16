//////////////////////////////////////////////////////////////
// File.    GameDebug.h
// Summary. GameDebugClass
// Date.    2018/10/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <iostream>
#include <fstream>
#include <sstream>

#include "../pch.h"
#include "../ExclusiveGameObject/SingletonBase.h"

class GameDebug : public SingletonBase<GameDebug>
{
// メンバー変数
public:

private:
	std::unique_ptr<DirectX::SpriteBatch>   m_sprites;       // スプライトバッチ
	std::unique_ptr<DirectX::SpriteFont>    m_font;          // スプライトフォント

// メンバー関数
public:
	~GameDebug();

	// フレンド関数
	friend SingletonBase<GameDebug>;

	// デバッグ描画
	void DebugRender(char* debugLen, DirectX::SimpleMath::Vector2 debugPos);
	void DebugIntRender(char* debugLen, int intValue, DirectX::SimpleMath::Vector2 debugPos);
	void DebugFloatRender(char* debugLen, float floatValue, DirectX::SimpleMath::Vector2 debugPos);
	void DebugBoolRender(char* debugLen, bool boolValue, DirectX::SimpleMath::Vector2 debugPos);

private:
	GameDebug();

};
