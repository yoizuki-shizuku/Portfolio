#include "GlitchEffect.hlsli"

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
void main(point PS_INPUT input[1],
    inout TriangleStream<PS_INPUT> output)
{
    for (unsigned int i = 0; i < vnum; i++)
    {
        //  ジオメトリ出力
        PS_INPUT element;
        
        //  受け取った情報を変数へ代入
        float2 pos = input[0].pos.xy;
        float offsetPos = input[0].pos.z;
        float2 scale = input[0].color.xy;
        uint anchor = input[0].color.z;
        float offsetWidth = input[0].color.a;
        float2 size = input[0].tex.xy;
                
        //  ピクセルから画面比率に沿った頂点の位置を計算
        element.pos.xy = scale * size * offset_vertex[i];
        
        //  アンカーポイントの位置へ原点を動かす
        element.pos.x -= (size.x * scale.x / 2.f) * (anchor % 3u);
        element.pos.y -= (size.y * scale.y / 2.f) * (anchor / 3u);
        
        //  回転させる
        //float2 original = element.pos.xy;
        //element.pos.x = (cos(radian) * original.x) + (-sin(radian) * original.y);
        //element.pos.y = (sin(radian) * original.x) + (cos(radian) * original.y);

        //  大きさがピクセル単位になるよう計算
        element.pos.x *= 2.f / windowSize.x;
        element.pos.y *= 2.f / windowSize.y;
        
        //  画面の左上基準にする
        element.pos.xy -= float2(1.f, 1.f);
        
        //  positionの位置へ移動させる
        pos /= windowSize.xy;
        pos *= 2.f;
        element.pos.xy += pos;
        
        //  y軸を反転
        element.pos.y *= -1;
                
        //XY平面なので zは0、wは1
        element.pos.z = 0.0f;
        element.pos.w = 1.0f;
        
        //  乱数を頂点に入れておく
        element.color = float4(offsetPos, offsetWidth, 1.f, 1.f);
        
        //  テクスチャのUV座標
        element.tex = offset_vertex[i].xy;
        
        output.Append(element);
    }
    output.RestartStrip();
}