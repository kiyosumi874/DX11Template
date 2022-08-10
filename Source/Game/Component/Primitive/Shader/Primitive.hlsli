cbuffer global
{
    matrix g_WVP;
    float4 g_color;
};


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};