//////////////////////////////////////////////////////////////
// File.    EffectManager.cpp
// Summary. EffectManagerClass
// Date.    2019/01/07
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////


// �C���N���[�h�f�B���N�g��
#include "../pch.h"
#include "EffectManager.h"
#include <WICTextureLoader.h>
#include "BinaryFile.h"
#include "d3d11.h"
#include <Effects.h>
#include <Model.h>

#include "../Utility/InputManager.h"
#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"

#include "../Utility/GameDebug.h"

// using�f�B���N�g��
using namespace DirectX::SimpleMath;
using namespace DirectX;

// ���̓��C�A�E�g�̏�����
const std::vector<D3D11_INPUT_ELEMENT_DESC> EffectManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// ����
/// </summary>
void EffectManager::Create()
{
	auto device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();

	// ���e�N�X�`���̓ǂݍ���
	const wchar_t* name = L"Resources\\Images\\ball.png";
	const wchar_t* name2 = L"Resources\\Images\\ball.png";
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), name, nullptr, m_texture1.GetAddressOf());
	DirectX::CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), name2, nullptr, m_texture2.GetAddressOf());

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0], INPUT_LAYOUT.size(),VSData.GetData(), VSData.GetSize(), m_inputLayout.GetAddressOf());
	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}
	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext());

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);
	m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"Resources");
	//m_model = Model::CreateFromCMO(device, L"Resources\\cup.cmo", *m_fxFactory);

	// �G�t�F�N�g�̏�����
	for (int i = 0; i < 1; i++) {
		MyEffect* effect = new MyEffect();
		m_effectList.push_back(effect);
	}

	// �ϐ��̏�����
	m_radius = 0.0f;
	m_white = 0.0f;

}
/// <summary>
/// �p��
/// </summary>
void EffectManager::Lost() {
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		delete (*itr);
	}
}

/// <summary>
/// ������
/// </summary>
/// <param name="life">�G�t�F�N�g�̕\������</param>
/// <param name="pos">�ʒu</param>
/// <param name="dir">����</param>
void EffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir)
{
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		Vector3 vel = dir;
		vel.Normalize();

		vel *= float(sin(rand()) + 1.0f);
		Vector3 raff = Vector3(vel.y, -vel.x, 0);
		raff *= float(sin(rand())*0.3f);
		vel = vel + raff;

		vel *= 0.1f;
		(*itr)->Initialize(life, pos, vel);
	}	
}
/// <summary>
/// �X�V
/// </summary>
/// <param name="timer">�o�ߎ���</param>
void EffectManager::Update(DX::StepTimer timer)
{
	InputManager::SingletonGetInstance().Update();

	m_timer = float(timer.GetTotalSeconds());

	for(std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end();itr++)
	{
		(*itr)->Update(timer);
	}

	//--------------------�L�[�{�[�h--------------------//
	if (InputManager::SingletonGetInstance().GetKeyState().Up)
	{
		//m_radius += 0.01f;
	}
	if (InputManager::SingletonGetInstance().GetKeyTracker().IsKeyReleased(DirectX::Keyboard::Down))
	{
		/*if(m_radius > 0.0f)
		m_radius -= 0.01f;*/
	}
	
	if (InputManager::SingletonGetInstance().GetKeyState().Space)
	{
		m_white += 0.005f;
	}
	else
	{
		if (m_white > 0.0f)
			m_white -= 0.005f;
	}

	//--------------------------------------------------//
}
/// <summary>
/// �\��
/// </summary>
void EffectManager::Render()
{
	m_vertex.clear();
	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		Vector3 pos = (*itr)->GetPosition();
		Vector3 vel = (*itr)->GetVelocity();

		{
			VertexPositionColorTexture vertex;
			vertex = VertexPositionColorTexture(pos, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
			m_vertex.push_back(vertex);
		}
	}

	Draw(Matrix::Identity,m_view, m_proj);
}

/// <summary>
/// 3D���f���̂��߂̃r���[�s��Ǝˉe�s���ݒ�
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void EffectManager::SetRenderState(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_view = view;
	m_proj = proj;
}

/// <summary>
/// �`��
/// </summary>
/// <param name="world">���[���h���W</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void EffectManager::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	RenderModel();

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	/*m_model->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
		Matrix::CreateRotationZ(10.0f), m_view, m_proj);*/

	//----------------------�}�E�X----------------------//
	RECT outputSize = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	int width = std::max<int>(outputSize.right - outputSize.left, 1);
	int height = std::max<int>(outputSize.bottom - outputSize.top, 1);

	int x = InputManager::SingletonGetInstance().GetMousePosX();
	int y = InputManager::SingletonGetInstance().GetMousePosY();

	float w, h;
	w = float(x) / float(width);
	h = float(y) / float(height);

	float scrlValue = float(InputManager::SingletonGetInstance().GetMouseState().scrollWheelValue);
	//--------------------------------------------------//

	ConstBuffer cbuff;

	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = Vector4((cosf(m_timer)+1), sinf(m_timer) ,m_timer, 0);
	cbuff.Mouse = Vector4(w, h, scrlValue, 0.0f);
	cbuff.Keyboard = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

	//GameDebug::SingletonGetInstance().DebugFloatRender("cosValue:", cbuff.Diffuse.x, SimpleMath::Vector2(10, 10));
	//GameDebug::SingletonGetInstance().DebugFloatRender("sinValue:", cbuff.Diffuse.y, SimpleMath::Vector2(10, 30));
	//GameDebug::SingletonGetInstance().DebugFloatRender("m_timer:", cbuff.Diffuse.z, SimpleMath::Vector2(10, 10));

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	ID3D11BlendState* blendstate = CommonStateManager::SingletonGetInstance().GetStates()->NonPremultiplied();
	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɎQ�Ƃ���
	context->OMSetDepthStencilState(CommonStateManager::SingletonGetInstance().GetStates()->DepthRead(), 0);
	// �J�����O�͍�����
	context->RSSetState(CommonStateManager::SingletonGetInstance().GetStates()->CullNone());

	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	ID3D11SamplerState* sampler[1] = { CommonStateManager::SingletonGetInstance().GetStates()->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
	// ���g�p����摜�̃V�F�[�_�[�������ŃZ�b�g����
	// ���e�N�X�`����m_srv�ɂ���ƃ��f���ɃG�t�F�N�g��������(��ʑS�̂�)
	context->PSSetShaderResources(0, 1, m_texture1.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());


	context->IASetInputLayout(m_inputLayout.Get());

	// ������������`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_batch->End();

}
/// <summary>
/// �`��(3D���f��)
/// </summary>
void EffectManager::RenderModel() 
{
	m_tex.Reset();
	m_rtv.Reset();
	m_srv.Reset();

	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	D3D11_TEXTURE2D_DESC texDesc;
	DX::DeviceResources::SingletonGetInstance().GetRenderTarget()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	DX::DeviceResources::SingletonGetInstance().GetD3DDevice()->CreateTexture2D(&texDesc, NULL, m_tex.GetAddressOf());

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	// �����_�[�^�[�Q�b�g�r���[�̐���
	DX::DeviceResources::SingletonGetInstance().GetD3DDevice()->CreateRenderTargetView(m_tex.Get(), &rtvDesc, &m_rtv);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

	auto hr = DX::DeviceResources::SingletonGetInstance().GetD3DDevice()->CreateShaderResourceView(
		m_tex.Get(), &srvDesc, m_srv.GetAddressOf());

	ID3D11RenderTargetView* defaultRTV = DX::DeviceResources::SingletonGetInstance().GetRenderTargetView();

	ID3D11DepthStencilView* pDSV = DX::DeviceResources::SingletonGetInstance().GetDepthStencilView();

	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
	context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), pDSV);
	context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
	/*m_model->Draw(DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext(), *CommonStateManager::SingletonGetInstance().GetStates(), 
	Matrix::Identity, m_view, m_proj);*/

	//clearColor[0] =0.3f;
	//clearColor[1] = 0.3f;


	context->OMSetRenderTargets(1, &defaultRTV, pDSV);
	context->ClearRenderTargetView(defaultRTV, Colors::CornflowerBlue);
	context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

}