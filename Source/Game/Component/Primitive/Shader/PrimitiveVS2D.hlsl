#include "Primitive.hlsli"

cbuffer global
{
    matrix g_W : packoffset(c0); //ワールド行列
    float g_ViewPortWidth : packoffset(c4); //ビューポート（スクリーン）横サイズ
    float g_ViewPortHeight : packoffset(c5); //ビューポート（スクリーン）縦サイズ
};


PS_INPUT VS(float4 pos : POSITION)
{
    PS_INPUT output;
    
    output.Pos = mul(pos, g_W);

    output.Pos.x = (output.Pos.x / g_ViewPortWidth) * 2 - 1;  // 2Dの座標に変換
    output.Pos.y = 1 - (output.Pos.y / g_ViewPortHeight) * 2; // 2Dの座標に変換
    
    return output;
}