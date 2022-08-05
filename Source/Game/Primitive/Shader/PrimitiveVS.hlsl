#include "Primitive.hlsli"

cbuffer global
{
    matrix g_WVP;
};


PS_INPUT VS(float4 pos : POSITION)
{
    PS_INPUT output;
    output.Pos = mul(pos, g_WVP);
    return output;
}