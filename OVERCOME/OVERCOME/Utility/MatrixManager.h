//////////////////////////////////////////////////////////////
// File.    MatrixManager.h
// Summary. MatrixManagerClass
// Date.    2018/10/31
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ

class MatrixManager
{
// メンバー変数
public:

private:
	DirectX::SimpleMath::Matrix   m_view;         // ビュー行列
	DirectX::SimpleMath::Matrix   m_projection;   // 射影行列

// メンバー関数
public:
	// コンストラクタ
	MatrixManager();
	// デストラクタ
	~MatrixManager();

	// リセット
	void ResetMatrix();	

	// Getter
	DirectX::SimpleMath::Matrix& GetView()       { return m_view; }
	DirectX::SimpleMath::Matrix& GetProjection() { return m_projection; }

	// Setter
	void SetViewProjection(DirectX::SimpleMath::Matrix& view, DirectX::SimpleMath::Matrix& projection) { m_view = view; m_projection = projection; }

private:

};