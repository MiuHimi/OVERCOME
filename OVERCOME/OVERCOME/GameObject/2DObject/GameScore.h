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

#include "../../pch.h"
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

#include "../../Game.h"

class GameScore
{
// メンバー変数
public:

private:
	static int                                           m_score;                       // プレイヤーが得る得点
	bool                                                 m_deductOccurrence = false;    // コースアウトしているかを判定
	int                                                  m_compareColum;                // プレイヤージャンプによる加点判定用変数
	int                                                  m_compareLine;                 // プレイヤージャンプによる加点判定用変数
	bool                                                 m_getPointChance = false;      // プレイヤージャンプによる加点判定用変数

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>     m_textureBsckground;           // テクスチャハンドル
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

	// 加点チャンス
	void AddPointChance(int j, int i);

	// Getter,Setter
	static int GetScore()                           { return m_score; }
	static void SetScore(int score)                 { m_score = score; }
	// 得点の増減
	void FluctuationScore(int addscore)      { m_score += addscore; }
	// プレイヤーがコースアウトしたらフラグが立つ
	void SetDeductFlag(bool flag)            { m_deductOccurrence = flag; }
	// プレイヤージャンプによる加点判定用の値設定
	void SetAddPointChance(int j, int i)     { m_compareColum = j; m_compareLine = i;}
	// プレイヤー得点獲得チャンスか判定
	bool GetPointChance()                    { return m_getPointChance; }
	// プレイヤー得点獲得チャンス設定
	void SetPointChance(bool flag)           { m_getPointChance = flag; }

private:

};