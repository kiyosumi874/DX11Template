cbuffer global
{
    matrix g_mW; //���[���h�s��
    matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
    float4 g_vLightDir; //���C�g�̕����x�N�g��
    float4 g_Diffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
    float4 g_eye;//�J�������_
};


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
    float3 Light : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float3 EyeVector : TEXCOORD2;
};