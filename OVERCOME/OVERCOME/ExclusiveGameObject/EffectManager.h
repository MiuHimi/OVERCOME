//////////////////////////////////////////////////////////////
// File.    EffectManager.h
// Summary. EffectManagerClass
// Date.    2019/01/07
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// インクルードディレクトリ
#include <SimpleMath.h>
#include "../Utility/StepTimer.h"
#include "MyEffect.h"
#include "Model.h"

#include <list>

class EffectManager
{
// メンバー変数
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
		DirectX::SimpleMath::Vector4	Mouse;
		DirectX::SimpleMath::Vector4	Keyboard;
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;       // 入力レイアウト

private:
	std::list<MyEffect*>		                       m_effectList;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_texture1;         // テクスチャへのポインタ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_texture2;         // テクスチャへのポインタ

	Microsoft::WRL::ComPtr<ID3D11Buffer>	           m_CBuffer;          // 定数バッファ

	Microsoft::WRL::ComPtr<ID3D11VertexShader>         m_VertexShader;     // 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader>          m_PixelShader;      // ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>       m_GeometryShader;   // ジオメトリシェーダ

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> 
		                                               m_batch;            // プリミティブバッチ

	Microsoft::WRL::ComPtr<ID3D11InputLayout>          m_inputLayout;      // 入力レイアウト
	std::vector<DirectX::VertexPositionColorTexture>   m_vertex;

	DirectX::SimpleMath::Matrix                        m_view;             // ビュー行列
	DirectX::SimpleMath::Matrix                        m_proj;             // 射影行列

	std::unique_ptr<DirectX::EffectFactory>            m_fxFactory;        // モデルエフェクト
	std::unique_ptr<DirectX::Model>                    m_model;            // モデルポインタ

	Microsoft::WRL::ComPtr<ID3D11Texture2D>            m_tex;              // テクスチャポインタ
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	   m_rtv;              // レンダーターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_srv;              // シェーダリソースビュー

	float                                              m_radius;           // 半径
	float                                              m_white;            // 白へと変わっていく変化値

private:
	float                                              m_timer;            // 時間取得

// メンバー関数
public:
	// 生成
	void Create();
	// 初期化
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir);
	// 更新
	void Update(DX::StepTimer timer);
	// 表示
	void Render();
	// 廃棄
	void Lost();

	// 3Dモデルのためのビュー行列と射影行列を設定
	void SetRenderState(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// 描画
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// 描画(3Dモデル)
	void RenderModel();
};