//////////////////////////////////////////////////////////////
// File.    String.h
// Summary. StringClass
// Date.    2019/01/29
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////
#pragma once

class Game;

// ��JIS�R�[�h���A�X�y�[�X����Z�܂ł̕`��
class String
{
	// �����̃T�C�Y
	static const int SIZE = 24;

	// �e�N�X�`���n���h��
	ID3D11ShaderResourceView* m_texture;

	// ������
	std::string m_str;

	// �\���ʒu
	DirectX::SimpleMath::Vector2 m_pos;

	// �����̐F
	DirectX::SimpleMath::Color m_color;

public:
	// �R���X�g���N�^
	String();

	// �������֐�
	void Initialize(ID3D11ShaderResourceView* texture, DirectX::SimpleMath::Vector2 pos, const char* str, DirectX::FXMVECTOR color = DirectX::Colors::White);

	// �`��֐�
	void Draw();

	// ������ݒ�֐�
	void SetStr(const char* str);

	// �\���ʒu�̐ݒ�֐�
	void SetPos(DirectX::SimpleMath::Vector2 pos);
};