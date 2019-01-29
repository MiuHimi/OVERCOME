//////////////////////////////////////////////////////////////
// File.    String.h
// Summary. StringClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////
#pragma once

class Game;

// ※JISコードより、スペースからZまでの描画
class String
{
	// 文字のサイズ
	static const int SIZE = 24;

	// テクスチャハンドル
	ID3D11ShaderResourceView* m_texture;

	// 文字列
	std::string m_str;

	// 表示位置
	DirectX::SimpleMath::Vector2 m_pos;

	// 文字の色
	DirectX::SimpleMath::Color m_color;

public:
	// コンストラクタ
	String();

	// 初期化関数
	void Initialize(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 pos, const char* str, DirectX::FXMVECTOR color = DirectX::Colors::White);

	// 描画関数
	void Draw();

	// 文字列設定関数
	void SetStr(const char* str);

	// 表示位置の設定関数
	void SetPos(DirectX::SimpleMath::Vector2 pos);
};