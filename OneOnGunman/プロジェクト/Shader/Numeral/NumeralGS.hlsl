#include "Numeral.hlsli"

static const unsigned int vnum = 4;

//  各頂点の位置
static const float2 offset_vertex[vnum] =
{
    float2(0.f, 1.f), //  左上
    float2(1.f, 1.f), //  右上
    float2(0.f, 0.f), //  左下
    float2(1.f, 0.f), //  右下
};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1], 
	inout TriangleStream< PS_INPUT > output
)
{
	for (unsigned int i = 0; i < vnum; i++)
	{
		//	ジオメトリ出力
        PS_INPUT element;
        
        //  受け取った情報を変数へ代入
        float2 pos = input[0].pos.xy;
        float radian = input[0].pos.z;
        float2 size = input[0].tex;
        
        //  ピクセル単位に沿った頂点の位置を計算
        element.pos.xy = size * offset_vertex[i];
        
        //  画像の中心が原点になるようにする
        element.pos.x -= size.x / 2.f;
        element.pos.y -= size.y / 2.f;
        
                //  回転させる
        float2 original = element.pos.xy;
        element.pos.x = (cos(radian) * original.x) + (-sin(radian) * original.y);
        element.pos.y = (sin(radian) * original.x) + (cos(radian) * original.y);

                //  大きさがピクセル単位になるよう計算
        element.pos.x *= 2.f / windowWidth;
        element.pos.y *= 2.f / windowHeight;
        
        //  画面の左上基準にする
        element.pos.xy -= float2(1.f, 1.f);
        
        //  positionの位置へ移動させる
        pos /= float2(windowWidth, windowHeight);
        pos *= 2.f;
        element.pos.xy += pos;
        
        //  y軸を反転
        element.pos.y *= -1;
                
        //XY平面なので zは0、wは1
        element.pos.z = 0.0f;
        element.pos.w = 1.0f;

        //  数字に合わせて画像を切り取るためにuv座標を計算
        element.tex.x = (0.1f * numeral) + (0.1f * offset_vertex[i].x);
        element.tex.y = offset_vertex[i].y;
        
        output.Append(element);
    }
    output.RestartStrip();
}