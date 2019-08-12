//////////////////////////////////////////////////////////////
// File.    EffectPS.hlsl
// Summary. PixelShader
// Date.    2019/08/12
// Auther.  Miu Himi
//////////////////////////////////////////////////////////////

// インクルードディレクトリ
#include "Effect.hlsli"

Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

/// <summary>
/// ノイズ(砂嵐)
/// </summary>
float random(float2 p)
{
	return frac(sin(dot(p, float2(12.9898f, 78.233f))) * 43758.5453f);
}
/// <summary>
/// ノイズ(砂嵐)強化版
/// </summary>
float2 random2(float2 st)
{
	st = float2(dot(st, float2(127.1f, 311.7f)),
		dot(st, float2(269.5f, 183.3f)));
	return -1.0f + 2.0f * frac(sin(st) * 43758.5453123f);
}

/// <summary>
/// // ノイズ(ブロック分割)
/// </summary>
float noise(float2 st)
{
	float2 p = floor(st);
	return random(p);
}

/// <summary>
/// ノイズ(少し境界を滑らかに)
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
/// ノイズ(perlinNoiseの強化版)(Fractal Brownian Motion)
/// </summary>
float fBm(float2 st)
{
	float f = 0;
	float2 q = st;

	// どれかをコメントアウトしないとメモリが足らない
	f += 0.5000f*perlinNoise(q); q = q *2.01f;
	f += 0.2500f*perlinNoise(q); q = q *2.02f;
	//f += 0.1250f*perlinNoise(q); q = q *2.03f;
	f += 0.0625f*perlinNoise(q); q = q *2.01f;

	return f;
}

/// <summary>
/// distortionを求める(歪みを求める)
/// </summary>
/// <param name="center">中心点</param>
/// <param name="r">半径</param>
/// <param name="d_r">歪みの幅</param>
/// <param name="tex">テクスチャ</param>
/// <returns>distortionの値</returns>
float distortion(float2 center, float r, float d_r, float2 tex)
{
	// 距離計測(中心点からピクセルの位置)
	float dist = distance(center, tex);
	// 歪ませる幅
	return 1 - smoothstep(r - d_r, r, dist);
}
/// <summary>
/// ポータル(穴)をあける
/// </summary>
/// <param name="center">中心点</param>
/// <param name="r">半径</param>
/// <param name="d_r">歪みの幅</param>
/// <param name="tx">テクスチャ</param>
/// <returns>穴が開いたテクスチャ情報</returns>
float4 portal(float2 center, float r, float d_r, float2 tx)
{
	// 穴の歪み
	float d = distortion(center, r, d_r, tx);
	// 画像のuv座標
	float2 uv = lerp(tx, center, d);

	// テクスチャのサンプリング
	float4 base = tex.Sample(samLinear, uv);
	// 画像に穴をあけた色情報
	return lerp(base, float4(0, 0, 0, 0), step(1, d));
}

/// <summary>
/// ブラーをかける(ぼやける)
/// </summary>
/// <param name="effectAmmount">割られるほう</param>
/// <param name="tx">割るほう</param>
/// <returns>ブラーのかかったテクスチャ情報</returns>
float4 blur(float2 effectAmmount, float2 tx)
{
	// ぼやける影響量
	float2 n = effectAmmount;
	// 座標を少しずつ八方向にずらす
	float4 base = tex.Sample(samLinear, tx + float2(n.x, n.y))
		+ tex.Sample(samLinear, tx + float2(n.x, 0))
		+ tex.Sample(samLinear, tx + float2(n.x, -n.y))
		+ tex.Sample(samLinear, tx + float2(0, n.y))
		+ tex.Sample(samLinear, tx + float2(0, 0))
		+ tex.Sample(samLinear, tx + float2(0, -n.y))
		+ tex.Sample(samLinear, tx + float2(-n.x, n.y))
		+ tex.Sample(samLinear, tx + float2(-n.x, 0))
		+ tex.Sample(samLinear, tx + float2(-n.x, -n.y));
	// 9で割って一枚分の色情報に
	base /= 9.0f;
	return base;
}
/// <summary>
/// テクスチャの解像度を変更する
/// (縦横比はそのまま)
/// </summary>
/// <param name="effectAmmountTmp">影響量</param>
/// <param name="effectAmmount">影響量</param>
/// <param name="tx">テクスチャ</param>
/// <returns>解像度を変更したuv情報</returns>
float2 resolution(float effectAmmountTmp, float effectAmmount, float2 tx)
{
	// 解像度分かけて同じ量で割る
	// 可変する場合はそれを割る
	float2 uv;
	uv.x = floor(tx.x * effectAmmountTmp * effectAmmount) / effectAmmountTmp / effectAmmount;
	uv.y = floor(tx.y * effectAmmountTmp*0.75f * effectAmmount) / (effectAmmountTmp*0.75f) / effectAmmount;

	return uv;
}

/// <summary>
/// グレースケールを作成
/// </summary>
/// <param name="colorinfo">歪みの幅</param>
/// <param name="tx">テクスチャ</param>
/// <returns>グレーの色情報</returns>
float grayscale(float4 colorinfo)
{
	return colorinfo.r*0.2f + colorinfo.g*0.7f + colorinfo.b*0.3f;
}
/// <summary>
/// 色調別の色情報を作成
/// (二枚目のテクスチャ情報を参照)
/// </summary>
/// <param name="colorinfo">テクスチャ</param>
/// <returns>色調情報</returns>
float4 colortone(float4 colorinfo)
{
	float g = grayscale(colorinfo);

	float2 colorUV = float2(g, g);
	return colorinfo = tex2.Sample(samLinear, colorUV);
}

/// <summary>
/// ディソルブ処理
/// </summary>
/// <param name="elapsedTime">経過時間</param>
/// <param name="tx">テクスチャ</param>
/// <returns>ディソルブ処理されたテクスチャ情報</returns>
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
/// 大きい値を小さい値で割ったときの余りを返す
/// </summary>
/// <param name="a">割られるほう</param>
/// <param name="b">割るほう</param>
/// <returns>あまり</returns>
float2 mod(float2 a, float2 b)
{
	return a - floor(a / b) * b;
}


float4 main(PS_INPUT input) : SV_TARGET
{
	float2 uv = input.Tex;
	uv = uv - float2(0.5f, 0.5f);

	// distance(float2(0,0),uvと同じ
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
