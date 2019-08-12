//////////////////////////////////////////////////////////////
// File.    EffectManager.h
// Summary. EffectManagerClass
// Date.    2019/08/12
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include <vector>
#include <list>

#include "../Utility/StepTimer.h"
#include "../Utility/DeviceResources.h"

class EffectManager
{
// メンバー変数
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;	// 時間経過等で取得した値を格納
		DirectX::SimpleMath::Vector4	Mouse;		// マウスで取得した値を格納
		DirectX::SimpleMath::Vector4	Keyboard;   // キーボードで取得した値を格納
	};

	struct MouseState
	{
		float cursorW;
		float cursorH;
		float scrlValue;
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC>	  INPUT_LAYOUT;     // インプットレイアウト
	
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	  m_texture1;		// テクスチャ情報
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	  m_texture2;	    // テクスチャ情報

	Microsoft::WRL::ComPtr<ID3D11Buffer>				  m_CBuffer;		// バッファ情報

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			  m_VertexShader;   // 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			  m_PixelShader;    // ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>		  m_GeometryShader; // ジオメトリシェーダ

	
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>
														  m_batch;			// プリミティブバッチ
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			  m_inputLayout;    // 入力レイアウト
	std::vector<DirectX::VertexPositionColorTexture>	  m_vertex;			// 頂点の位置色テクスチャ情報

	DirectX::SimpleMath::Matrix							  m_view;			// ビュー行列
	DirectX::SimpleMath::Matrix							  m_proj;			// 射影行列

	float												  m_timer;			// 時間経過計測

	MouseState											  m_mouseState;		// マウスの状態

// メンバー関数
public:
	// 初期化
	void Initialize();
	// 生成
	void Create();
	// 更新
	void Update(DX::StepTimer timer);
	// 描画
	void Render();
	// 終了
	void Finalize();

	// ビュー行列と射影行列を設定
	void SetRenderState(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	// 描画
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

private:
	
};