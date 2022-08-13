cbuffer global
{
    matrix g_mW; //ワールド行列
    matrix g_mWVP; //ワールドから射影までの変換行列
    float4 g_vLightDir; //ライトの方向ベクトル
    float4 g_Diffuse = float4(1, 0, 0, 0); //拡散反射(色）
    float4 g_eye;//カメラ視点
};


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
    float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
};