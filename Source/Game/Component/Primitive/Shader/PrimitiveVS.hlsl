#include "Primitive.hlsli"


PS_INPUT VS(float4 pos : POSITION)
{
    PS_INPUT output;
    output.Pos = mul(pos, g_WVP);
    return output;
}