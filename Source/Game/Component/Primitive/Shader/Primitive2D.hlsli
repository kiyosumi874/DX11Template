
cbuffer global2D
{
    matrix g_W : packoffset(c0); //ワールド行列
    float4 g_color : packoffset(c4); //色
    float g_ViewPortWidth : packoffset(c5); //ビューポート（スクリーン）横サイズ
    float g_ViewPortHeight : packoffset(c6); //ビューポート（スクリーン）縦サイズ
};


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};