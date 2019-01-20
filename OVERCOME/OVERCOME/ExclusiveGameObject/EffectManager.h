//////////////////////////////////////////////////////////////
// File.    EffectManager.h
// Summary. EffectManagerClass
// Date.    2019/01/07
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
#include <SimpleMath.h>
#include "../Utility/StepTimer.h"
#include "MyEffect.h"
#include "Model.h"

#include <list>

class EffectManager
{
// �����o�[�ϐ�
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

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;       // ���̓��C�A�E�g

private:
	std::list<MyEffect*>		                       m_effectList;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_texture1;         // �e�N�X�`���ւ̃|�C���^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_texture2;         // �e�N�X�`���ւ̃|�C���^

	Microsoft::WRL::ComPtr<ID3D11Buffer>	           m_CBuffer;          // �萔�o�b�t�@

	Microsoft::WRL::ComPtr<ID3D11VertexShader>         m_VertexShader;     // ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>          m_PixelShader;      // �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>       m_GeometryShader;   // �W�I���g���V�F�[�_

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> 
		                                               m_batch;            // �v���~�e�B�u�o�b�`

	Microsoft::WRL::ComPtr<ID3D11InputLayout>          m_inputLayout;      // ���̓��C�A�E�g
	std::vector<DirectX::VertexPositionColorTexture>   m_vertex;

	DirectX::SimpleMath::Matrix                        m_view;             // �r���[�s��
	DirectX::SimpleMath::Matrix                        m_proj;             // �ˉe�s��

	std::unique_ptr<DirectX::EffectFactory>            m_fxFactory;        // ���f���G�t�F�N�g
	std::unique_ptr<DirectX::Model>                    m_model;            // ���f���|�C���^

	Microsoft::WRL::ComPtr<ID3D11Texture2D>            m_tex;              // �e�N�X�`���|�C���^
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	   m_rtv;              // �����_�[�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   m_srv;              // �V�F�[�_���\�[�X�r���[

	float                                              m_radius;           // ���a
	float                                              m_white;            // ���ւƕς���Ă����ω��l

private:
	float                                              m_timer;            // ���Ԏ擾

// �����o�[�֐�
public:
	// ����
	void Create();
	// ������
	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir);
	// �X�V
	void Update(DX::StepTimer timer);
	// �\��
	void Render();
	// �p��
	void Lost();

	// 3D���f���̂��߂̃r���[�s��Ǝˉe�s���ݒ�
	void SetRenderState(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// �`��
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
	// �`��(3D���f��)
	void RenderModel();
};