#include "Primitive.hlsli"


PS_INPUT VS(float4 pos : POSITION, float4 normal : NORMAL)
{
    PS_INPUT output = (PS_INPUT) 0;

    output.Pos = mul(pos, g_mWVP);
    output.Normal = mul(normal, (float3x3) g_mW);
    output.Light = g_vLightDir;

    float3 PosWorld = mul(pos, g_mW);
    output.EyeVector = g_eye - PosWorld;

    return output;
}