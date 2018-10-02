//////////////////////////////////////////////////////////////
// File.    GameScore.h
// Summary. GameScoreClass
// Date.    2018/10/02
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <WICTextureLoader.h>
#include <CommonStates.h>

#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

class GameScore
{
// メンバー変数
public:

private:
	int m_score;
	int m_addScore;
	int m_count;

	std::unique_ptr<DirectX::SpriteBatch>                m_sprites;                     // スプライトバッチ
	std::unique_ptr<DirectX::CommonStates>               m_states;                      // コモンステート
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_texture;                     // テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureNum[10];              // テクスチャハンドル

// メンバー関数
public:
	// コンストラクタ
	GameScore();
	// デストラクタ
	~GameScore();

	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer);
	// 描画
	void Render();

	// Getter,Setter
	int GetScore() { return m_score; }
	void SetScore(int score) { m_score = score; }
	void FluctuationScore(int addscore) { m_score += addscore; }

private:

};