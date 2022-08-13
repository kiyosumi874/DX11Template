//グローバル

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer global
{
    matrix world : packoffset(c0); // ワールド行列
    float viewPortWidth : packoffset(c4); //ビューポート（スクリーン）横サイズ
    float viewPortHeight : packoffset(c5); //ビューポート（スクリーン）縦サイズ
};

//構造体
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 UV : TEXCOORD;
};

//
//
//バーテックスシェーダー
PS_INPUT VS(float4 pos : POSITION, float2 UV : TEXCOORD)
{
    PS_INPUT output;

    output.position = mul(pos, world);

    output.position.x = (output.position.x / viewPortWidth) * 2 - 1;
    output.position.y = 1 - (output.position.y / viewPortHeight) * 2;

    output.UV = UV;

    return output;
}
//
//
//ピクセルシェーダー
float4 PS(PS_INPUT input) : SV_Target
{
    return g_texture.Sample(g_sampler, input.UV);
}