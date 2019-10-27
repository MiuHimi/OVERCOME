//////////////////////////////////////////////////////////////
// File.    GameDecorateObject.h
// Summary. GameDecorateObjectClass
// Date.    2019/10/23
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <Model.h>
#include "../../Utility/DeviceResources.h"
#include "../../Utility/StepTimer.h"

class MatrixManager;
class GameDecorateObject
{
// メンバー変数
public:

private:
	DirectX::SimpleMath::Matrix				m_world;						// ワールド座標

	static const int						MAX_DOOR_NUM = 6;				// ドアの数
	DirectX::SimpleMath::Matrix				m_doorRota[MAX_DOOR_NUM];		// ドアオブジェクトの回転行列
	DirectX::SimpleMath::Vector3			m_doorTrans[MAX_DOOR_NUM];		// ドアオブジェクトの移動行列
	bool									m_isMoveDoor[MAX_DOOR_NUM];		// ドアが動き出したかどうか
	bool									m_isShowDoor[MAX_DOOR_NUM];		// ドアを表示できる状態かどうか

	std::unique_ptr<DirectX::Model>			mp_modelEnemyDoor[6];			// 敵が出てくるドアモデルオブジェクト

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

private:
	/// <summary>
	/// 描画前にするワールド行列の設定
	/// </summary>
	/// <param name="rota">回転行列</param>
	/// <param name="trans">移動行列</param>
	DirectX::SimpleMath::Matrix SetWorldMatrix(DirectX::SimpleMath::Matrix& world, DirectX::SimpleMath::Matrix rota, DirectX::SimpleMath::Matrix trans);

};