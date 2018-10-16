//////////////////////////////////////////////////////////////
// File.    SkyDome.h
// Summary. SkyDomeClass
// Date.    2018/09/15
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../Utility/DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"


class Game;
class SkyDome
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelSky;   // スカイドームモデルハンドル
	
	Game* mp_game;                                // Gameファイルの情報を格納

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	SkyDome(Game* game);
	// デストラクタ
	~SkyDome();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer, Game* game);
	// 描画
	void Render(DirectX::SimpleMath::Matrix view);
	// 廃棄処理
	void Depose();

private:


};