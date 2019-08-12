//////////////////////////////////////////////////////////////
// File.    EffectPS.hlsl
// Summary. PixelShader
// Date.    2019/08/12
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// �C���N���[�h�f�B���N�g��
#include "Effect.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

/// <summary>
/// �m�C�Y(����)
/// </summary>
float random(float2 p)
{
	return frac(sin(dot(p, float2(12.9898f, 78.233f))) * 43758.5453f);
}
/// <summary>
/// �m�C�Y(����)������
/// </summary>
float2 random2(float2 st)
{
	st = float2(dot(st, float2(127.1f, 311.7f)),
		dot(st, float2(269.5f, 183.3f)));
	return -1.0f + 2.0f * frac(sin(st) * 43758.5453123f);
}

/// <summary>
/// // �m�C�Y(�u���b�N����)
/// </summary>
float noise(float2 st)
{
	float2 p = floor(st);
	return random(p);
}

/// <summary>
/// �m�C�Y(�������E�����炩��)
/// </summary>
float perlinNoise(float2 st)
{
	float2 p = floor(st);
	float2 f = frac(st);
	float2 u = f * f * (3.0f - 2.0f * f);

	float v00 = random2(p + float2(0, 0));
	float v10 = random2(p + float2(1, 0));
	float v01 = random2(p + float2(0, 1));
	float v11 = random2(p + float2(1, 1));

	return lerp(lerp(dot(v00, f - float2(0, 0)), dot(v10, f - float2(1, 0)), u.x),
		lerp(dot(v01, f - float2(0, 1)), dot(v11, f - float2(1, 1)), u.x),
		u.y) + 0.5f;
}
/// <summary>
/// �m�C�Y(perlinNoise�̋�����)(Fractal Brownian Motion)
/// </summary>
float fBm(float2 st)
{
	float f = 0;
	float2 q = st;

	// �ǂꂩ���R�����g�A�E�g���Ȃ��ƃ�����������Ȃ�
	f += 0.5000f*perlinNoise(q); q = q *2.01f;
	f += 0.2500f*perlinNoise(q); q = q *2.02f;
	//f += 0.1250f*perlinNoise(q); q = q *2.03f;
	f += 0.0625f*perlinNoise(q); q = q *2.01f;

	return f;
}

/// <summary>
/// distortion�����߂�(�c�݂����߂�)
/// </summary>
/// <param name="center">���S�_</param>
/// <param name="r">���a</param>
/// <param name="d_r">�c�݂̕�</param>
/// <param name="tex">�e�N�X�`��</param>
/// <returns>distortion�̒l</returns>
float distortion(float2 center, float r, float d_r, float2 tex)
{
	// �����v��(���S�_����s�N�Z���̈ʒu)
	float dist = distance(center, tex);
	// �c�܂��镝
	return 1 - smoothstep(r - d_r, r, dist);
}
/// <summary>
/// �|�[�^��(��)��������
/// </summary>
/// <param name="center">���S�_</param>
/// <param name="r">���a</param>
/// <param name="d_r">�c�݂̕�</param>
/// <param name="tx">�e�N�X�`��</param>
/// <returns>�����J�����e�N�X�`�����</returns>
float4 portal(float2 center, float r, float d_r, float2 tx)
{
	// ���̘c��
	float d = distortion(center, r, d_r, tx);
	// �摜��uv���W
	float2 uv = lerp(tx, center, d);

	// �e�N�X�`���̃T���v�����O
	float4 base = tex.Sample(samLinear, uv);
	// �摜�Ɍ����������F���
	return lerp(base, float4(0, 0, 0, 0), step(1, d));
}

/// <summary>
/// �u���[��������(�ڂ₯��)
/// </summary>
/// <param name="effectAmmount">������ق�</param>
/// <param name="tx">����ق�</param>
/// <returns>�u���[�̂��������e�N�X�`�����</returns>
float4 blur(float2 effectAmmount, float2 tx)
{
	// �ڂ₯��e����
	float2 n = effectAmmount;
	// ���W���������������ɂ��炷
	float4 base = tex.Sample(samLinear, tx + float2(n.x, n.y))
		+ tex.Sample(samLinear, tx + float2(n.x, 0))
		+ tex.Sample(samLinear, tx + float2(n.x, -n.y))
		+ tex.Sample(samLinear, tx + float2(0, n.y))
		+ tex.Sample(samLinear, tx + float2(0, 0))
		+ tex.Sample(samLinear, tx + float2(0, -n.y))
		+ tex.Sample(samLinear, tx + float2(-n.x, n.y))
		+ tex.Sample(samLinear, tx + float2(-n.x, 0))
		+ tex.Sample(samLinear, tx + float2(-n.x, -n.y));
	// 9�Ŋ����Ĉꖇ���̐F����
	base /= 9.0f;
	return base;
}
/// <summary>
/// �e�N�X�`���̉𑜓x��ύX����
/// (�c����͂��̂܂�)
/// </summary>
/// <param name="effectAmmountTmp">�e����</param>
/// <param name="effectAmmount">�e����</param>
/// <param name="tx">�e�N�X�`��</param>
/// <returns>�𑜓x��ύX����uv���</returns>
float2 resolution(float effectAmmountTmp, float effectAmmount, float2 tx)
{
	// �𑜓x�������ē����ʂŊ���
	// �ς���ꍇ�͂��������
	float2 uv;
	uv.x = floor(tx.x * effectAmmountTmp * effectAmmount) / effectAmmountTmp / effectAmmount;
	uv.y = floor(tx.y * effectAmmountTmp*0.75f * effectAmmount) / (effectAmmountTmp*0.75f) / effectAmmount;

	return uv;
}

/// <summary>
/// �O���[�X�P�[�����쐬
/// </summary>
/// <param name="colorinfo">�c�݂̕�</param>
/// <param name="tx">�e�N�X�`��</param>
/// <returns>�O���[�̐F���</returns>
float grayscale(float4 colorinfo)
{
	return colorinfo.r*0.2f + colorinfo.g*0.7f + colorinfo.b*0.3f;
}
/// <summary>
/// �F���ʂ̐F�����쐬
/// (�񖇖ڂ̃e�N�X�`�������Q��)
/// </summary>
/// <param name="colorinfo">�e�N�X�`��</param>
/// <returns>�F�����</returns>
float4 colortone(float4 colorinfo)
{
	float g = grayscale(colorinfo);

	float2 colorUV = float2(g, g);
	return colorinfo = tex2.Sample(samLinear, colorUV);
}

/// <summary>
/// �f�B�\���u����
/// </summary>
/// <param name="elapsedTime">�o�ߎ���</param>
/// <param name="tx">�e�N�X�`��</param>
/// <returns>�f�B�\���u�������ꂽ�e�N�X�`�����</returns>
float4 dissolve(float elapsedTime, float2 tx)
{
	float n = noise(tx * 10);
	float4 base = tex.Sample(samLinear, tx);

	if (n < elapsedTime)
	{
		base.a = 0;
	}

	return base;
}

/// <summary>
/// �傫���l���������l�Ŋ������Ƃ��̗]���Ԃ�
/// </summary>
/// <param name="a">������ق�</param>
/// <param name="b">����ق�</param>
/// <returns>���܂�</returns>
float2 mod(float2 a, float2 b)
{
	return a - floor(a / b) * b;
}


float4 main(PS_INPUT input) : SV_TARGET
{
	float2 uv = input.Tex;
	uv = uv - float2(0.5f, 0.5f);

	// distance(float2(0,0),uv�Ɠ���
	float vignet = length(uv);

	float2 texUV = uv + float2(0.5f, 0.5f);

	float4 col = float4(0, 0, 0, 1);

	texUV.x += sin(texUV.y * 100 + 0.2f) * 0.002f;
	texUV += float2(0.001f, 0.001f);
	texUV.x += (random(floor(texUV.y * 500) + diffuse.y) - 0.5f) * 0.01f;

	texUV = mod(texUV, 1);

	col.r = tex.Sample(samLinear, texUV).r;
	col.g = tex.Sample(samLinear, texUV + float2(0.002f, 0)).g;
	col.b = tex.Sample(samLinear, texUV + float2(0.004f, 0)).b;

	if (random(
		(random(floor(texUV.y * 500) + float2(diffuse.y, diffuse.y)) - 0.5f) + float2(diffuse.y, diffuse.y)
	) < 0.1f)
	{
		col.r = random(uv + float2(123 + diffuse.y, 0));
		col.g = random(uv + float2(123 + diffuse.y, 1));
		col.b = random(uv + float2(123 + diffuse.y, 2));
	}

	col *= 1 - vignet * 1.3f;

	float4 base = tex.Sample(samLinear, texUV);
	base = float4(col.rgb, base.a);
	return base;
}
