//////////////////////////////////////////////////////////////
// File.    GameFloor.h
// Summary. GameFloorClass
// Date.    2018/07/27
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "../DeviceResources.h"
#include "../StepTimer.h"

#include "../Game.h"

#include "../ExclusiveGameObject/CollisionBox.h"

class Game;
class GameFloor : public CollisionBox
{
// メンバー変数(構造体、enum、列挙子 etc...)
public:

private:
	std::unique_ptr<DirectX::Model> m_modelLattice;   // 格子柄床モデルハンドル
	//std::unique_ptr<CollisionBox> mp_floor;         // 床衝突判定用オブジェクト

	Game* mp_game;                                    // Gameファイルの情報を格納

// メンバー関数(関数、Getter、Setter)
public:
	// コンストラクタ
	GameFloor(Game* game);
	// デストラクタ
	~GameFloor();

	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	bool Update(DX::StepTimer const& timer, Game* game);
	// 描画
	void Render(Game* game, DirectX::SimpleMath::Matrix view);
	// 廃棄処理
	void Depose();

private:


};