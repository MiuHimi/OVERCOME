//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.h
// Summary. GameDecorateObjectClass
// Date.    2019/10/23
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <array>
#include <Model.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameDecorateObject
{
// メンバー変数
public:
	static const int						MAX_CHEST_NUM = 3;				// チェストの最大数

private:
	DirectX::SimpleMath::Matrix				m_world;						// ワールド座標

	static const int						MAX_DOOR_NUM = 6;				// ドアの数
	DirectX::SimpleMath::Matrix				m_doorRota[MAX_DOOR_NUM];		// ドアオブジェクトの回転行列
	DirectX::SimpleMath::Vector3			m_doorTrans[MAX_DOOR_NUM];		// ドアオブジェクトの移動行列
	bool									m_isMoveDoor[MAX_DOOR_NUM];		// ドアが動き出したかどうか
	bool									m_isShowDoor[MAX_DOOR_NUM];		// ドアを表示できる状態かどうか

	DirectX::SimpleMath::Matrix				m_chestRota[MAX_CHEST_NUM];		// チェストオブジェクトの回転行列
	DirectX::SimpleMath::Vector3			m_chestTrans[MAX_CHEST_NUM];	// チェストオブジェクトの移動行列
	DirectX::SimpleMath::Vector3			m_chestTempPos[MAX_CHEST_NUM];	// チェストオブジェクトの基本位置
	float									m_chestHeight;					// チェストオブジェクトの高さ
	int										m_shakeCount[MAX_CHEST_NUM];	// チェストが振動するまでのカウント
	int										m_shakeVelCount[MAX_CHEST_NUM];	// チェストがある方向に振動しているカウント
	int										m_shakeNeedCount[MAX_CHEST_NUM];// チェストが振動するまでに必要なカウント
	bool									m_isChestOpen[MAX_CHEST_NUM];	// チェストが開閉しているかどうか
	bool									m_isChestSound[MAX_CHEST_NUM];	// チェストの開閉SEを再生したかどうか
	static const int						SHAKE_COUNT;					// 一方向に振動するカウント
	static const DirectX::SimpleMath::Vector3
											SHAKE_VEL;						// 振動する方向

	std::unique_ptr<DirectX::Model>			mp_modelEnemyDoor[MAX_DOOR_NUM];// 敵が出てくるドアモデルオブジェクト
	std::unique_ptr<DirectX::Model>			mp_modelSmallRoom;				// 小部屋モデルオブジェクト
	std::unique_ptr<DirectX::Model>			mp_modelEnemyChestOpen[MAX_CHEST_NUM];		// 敵が出てくるチェストモデルオブジェクト
	std::unique_ptr<DirectX::Model>			mp_modelEnemyChestClose[MAX_CHEST_NUM];		// 敵が出てくるチェストモデルオブジェクト

// メンバー関数
public:
	// コンストラクタ
	GameDecorateObject();
	// デストラクタ
	~GameDecorateObject();

	/// <summary>
	/// 生成
	/// </summary>
	void Create();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	/// <returns>終了状態</returns>
	bool Update(int roadID);
	/// <summary>
	/// 描画
	/// </summary>
	void Render(MatrixManager* matrixManager);
	/// <summary>
	/// 廃棄処理
	/// </summary>
	void Depose();

	//--------------------Getter--------------------//

	// チェストの位置を取得
	DirectX::SimpleMath::Vector3*	GetChestPos()		{ return m_chestTrans; }

	// チェストの高さを取得
	float							GetChestHeight()	{ return m_chestHeight; }

	// チェストの開閉状態を取得
	std::array<bool, 3>				GetChestIsOpen()	{ return{ m_isChestOpen[0], m_isChestOpen[1], m_isChestOpen[2] }; }

	//----------------------------------------------//

private:
	/// <summary>
	/// 描画前にするワールド行列の設定
	/// </summary>
	/// <param name="rota">回転行列</param>
	/// <param name="trans">移動行列</param>
	DirectX::SimpleMath::Matrix SetWorldMatrix(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix rota, DirectX::SimpleMath::Matrix trans);

	/// <summary>
	/// 敵出現ドアの更新
	/// </summary>
	/// <param name="roadID">道ID</param>
	void UpdateEnemyDoor(int roadID);

	/// <summary>
	/// 敵出現チェストの更新
	/// </summary>
	/// <param name="roadID">道ID</param>
	void UpdateEnemyChest(int roadID);
};