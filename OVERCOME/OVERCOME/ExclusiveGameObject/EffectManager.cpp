//////////////////////////////////////////////////////////////
// File.    EffectManager.cpp
// Summary. EffectManagerClass
// Date.    2019/08/12
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "../../pch.h"
#include "EffectManager.h"
#include <WICTextureLoader.h>
#include "BinaryFile.h"
#include "d3d11.h"
#include <Effects.h>

#include "../Utility/DeviceResources.h"
#include "../Utility/CommonStateManager.h"
#include "../Utility/InputManager.h"

// using�f�B���N�g��
using namespace DirectX::SimpleMath;
using namespace DirectX;

// const�f�B���N�g��
const std::vector<D3D11_INPUT_ELEMENT_DESC> EffectManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};


/// <summary>
/// ������
/// </summary>
void EffectManager::Initialize()
{
	// �}�E�X�̏�ԏ�����
	m_mouseState.cursorW = 0.0f;
	m_mouseState.cursorH = 0.0f;
	m_mouseState.scrlValue = 0.0f;
}
/// <summary>
/// ����
/// </summary>
void EffectManager::Create()
{
	auto device = DX::DeviceResources::SingletonGetInstance().GetD3DDevice();

	// �e�N�X�`���̓ǂݍ���
	const wchar_t* texPath1 = L"Resources\\Images\\black_alpha_half.png";
	const wchar_t* texPath2 = L"Resources\\Images\\dis.jpg";
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), texPath1, nullptr, m_texture1.GetAddressOf());
	CreateWICTextureFromFile(DX::DeviceResources::SingletonGetInstance().GetD3DDevice(), texPath2, nullptr, m_texture2.GetAddressOf());

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/EffectVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/EffectGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/EffectPS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
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
}
/// <summary>
/// �I��
/// </summary>
void EffectManager::Finalize()
{
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="timer"></param>
void EffectManager::Update(DX::StepTimer timer)
{
	// �b���擾
	m_timer = float(timer.GetTotalSeconds());

	// �}�E�X�̏�ԍX�V
	RECT outputSize = DX::DeviceResources::SingletonGetInstance().GetOutputSize();
	int width = std::max<int>(outputSize.right - outputSize.left, 1);
	int height = std::max<int>(outputSize.bottom - outputSize.top, 1);

	int x = InputManager::SingletonGetInstance().GetMousePosX();
	int y = InputManager::SingletonGetInstance().GetMousePosY();

	// ��ʂɑ΂��đ��ΓI�Ȉʒu(0�`1)
	m_mouseState.cursorW = float(x) / float(width);
	m_mouseState.cursorH = float(y) / float(height);
	// �z�C�[���̉�]�x
	m_mouseState.scrlValue = float(InputManager::SingletonGetInstance().GetMouseState().scrollWheelValue);
}
/// <summary>
/// �`��
/// </summary>
void EffectManager::Render()
{
	m_vertex.clear();
	VertexPositionColorTexture vertex;
	vertex = VertexPositionColorTexture(Vector3(0.0f, 0.0f, 0.0f), Vector4(0.0f, 0.0f, 0.0f, 1), Vector2(0.0f, 3.0f));
	m_vertex.push_back(vertex);

	Draw(Matrix::Identity, m_view, m_proj);
}

/// <summary>
/// �r���[�s��Ǝˉe�s���ݒ�
/// </summary>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�ˉe�s��</param>
void EffectManager::SetRenderState(SimpleMath::Matrix view, SimpleMath::Matrix projection)
{
	m_view = view;
	m_proj = projection;
}

/// <summary>
/// �`��
/// </summary>
/// <param name="world">���[���h�s��</param>
/// <param name="view">�r���[�s��</param>
/// <param name="projection">�ˉe�s��</param>
void EffectManager::Draw(SimpleMath::Matrix world, SimpleMath::Matrix view, SimpleMath::Matrix projection)
{
	auto context = DX::DeviceResources::SingletonGetInstance().GetD3DDeviceContext();

	ConstBuffer cbuff;

	cbuff.matView = view.Transpose();
	cbuff.matProj = projection.Transpose();
	cbuff.matWorld = world.Transpose();
	cbuff.Diffuse = Vector4((cosf(m_timer) + 1), sinf(m_timer), m_timer, 0);
	cbuff.Mouse = Vector4(m_mouseState.cursorW, m_mouseState.cursorH, m_mouseState.scrlValue, 0.0f);
	cbuff.Keyboard = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

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
	// �g�p����摜�̃V�F�[�_�[�������ŃZ�b�g����
	context->PSSetShaderResources(0, 1, m_texture1.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture2.GetAddressOf());

	context->IASetInputLayout(m_inputLayout.Get());

	// ������������`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_batch->End();

	// 2D�X�v���C�g�����ɕ`�悷��ꍇ���ɖ߂��Ȃ��ƃG���[�ɂȂ�
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
