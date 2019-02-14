#include "Particle.hlsli"
static const int vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.5f, -0.5f, 0, 0),	// 左下
	float4(-0.5f, +0.5f, 0, 0),	// 左上
	float4(+0.5f, -0.5f, 0, 0),	// 右上
	float4(+0.5f, +0.5f, 0, 0),	// 右下

};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream< PS_INPUT > output
)
{
	for (uint i = 0; i < vnum; i++)
	{
		PS_INPUT element;

		// ×2にすると画面全体にエフェクトが表示される(x,yのみ)
		element.Pos = input[0].Pos + float4(offset_array[i].x*2, offset_array[i].y*2, offset_array[i].z, offset_array[i].w);

		/*element.Pos = mul(element.Pos, matWorld);
		element.Pos = mul(element.Pos, matView);
		element.Pos = mul(element.Pos, matProj);*/

		element.Color = input[0].Color;
		
		// ☆表示をｘとｙで分ける
		element.Tex = float2(offset_array[i].x, -offset_array[i].y)+float2(0.5f,0.5f);
		output.Append(element);
	}
	output.RestartStrip();
}