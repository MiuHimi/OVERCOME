//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/22
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include "../pch.h"
#include "SingletonBase.h"

class MatrixManager : public SingletonBase<MatrixManager>
{
// メンバー変数
public:

private:
	static DirectX::SimpleMath::Matrix   m_view;         // ビュー行列
	static DirectX::SimpleMath::Matrix   m_projection;   // 射影行列

// メンバー関数
public:
	// デストラクタ
	~MatrixManager();

	// フレンド関数
	friend SingletonBase<MatrixManager>;

	// リセット
	void ResetMatrix();
	

	// Getter
	static DirectX::SimpleMath::Matrix& GetView()       { return m_view; }
	static DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// Setter
	static void SetView(DirectX::SimpleMath::Matrix& view)             { m_view = view; }
	static void SetProjection(DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }

private:
	// コンストラクタ
	MatrixManager();

};