//////////////////////////////////////////////////////////////
// File.    EffectManager.h
// Summary. EffectManagerClass
// Date.    2019/08/12
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

#pragma once

// �C���N���[�h�f�B���N�g��
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
// �����o�[�ϐ�
public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;	// ���Ԍo�ߓ��Ŏ擾�����l���i�[
		DirectX::SimpleMath::Vector4	Mouse;		// �}�E�X�Ŏ擾�����l���i�[
		DirectX::SimpleMath::Vector4	Keyboard;   // �L�[�{�[�h�Ŏ擾�����l���i�[
	};

	struct MouseState
	{
		float cursorW;
		float cursorH;
		float scrlValue;
	};

	static const std::vector<D3D11_INPUT_ELEMENT_DESC>	  INPUT_LAYOUT;     // �C���v�b�g���C�A�E�g
	
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	  m_texture1;		// �e�N�X�`�����
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	  m_texture2;	    // �e�N�X�`�����

	Microsoft::WRL::ComPtr<ID3D11Buffer>				  m_CBuffer;		// �o�b�t�@���

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			  m_VertexShader;   // ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			  m_PixelShader;    // �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>		  m_GeometryShader; // �W�I���g���V�F�[�_

	
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>
														  m_batch;			// �v���~�e�B�u�o�b�`
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			  m_inputLayout;    // ���̓��C�A�E�g
	std::vector<DirectX::VertexPositionColorTexture>	  m_vertex;			// ���_�̈ʒu�F�e�N�X�`�����

	DirectX::SimpleMath::Matrix							  m_view;			// �r���[�s��
	DirectX::SimpleMath::Matrix							  m_proj;			// �ˉe�s��

	float												  m_timer;			// ���Ԍo�ߌv��

	MouseState											  m_mouseState;		// �}�E�X�̏��

// �����o�[�֐�
public:
	// ������
	void Initialize();
	// ����
	void Create();
	// �X�V
	void Update(DX::StepTimer timer);
	// �`��
	void Render();
	// �I��
	void Finalize();

	// �r���[�s��Ǝˉe�s���ݒ�
	void SetRenderState(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
	// �`��
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

private:
	
};