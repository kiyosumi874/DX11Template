#include "Primitive.hlsli"

float4 PS(PS_INPUT input) : SV_Target
{
    float3 Normal = normalize(input.Normal);
    float3 LightDir = normalize(input.Light);
    float3 ViewDir = normalize(input.EyeVector);
    float4 NL = saturate(dot(Normal, LightDir));

    float3 Reflect = normalize(2 * NL * Normal - LightDir);
    float4 specular = 2 * pow(saturate(dot(Reflect, ViewDir)), 2);

    return g_Diffuse * NL + specular;
}