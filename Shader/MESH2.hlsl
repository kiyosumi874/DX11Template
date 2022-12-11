//�O���[�o��
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//�O���[�o��
cbuffer global_0:register(b0)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_vLightDir;  //���C�g�̕����x�N�g��
	float4 g_vEye;//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient=float4(0,0,0,0);//�A���r�G���g��
	float4 g_Diffuse=float4(1,0,0,0); //�g�U����(�F�j
	float4 g_Specular=float4(1,1,1,1);//���ʔ���
};

//�o�[�e�b�N�X�V�F�[�_�[�o�͍\����
struct VS_OUTPUT
{	
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
};
//
//�o�[�e�b�N�X�V�F�[�_�[
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//�ˉe�ϊ��i���[���h���r���[���v���W�F�N�V�����j
	//�@�������[���h��Ԃ�
	output.Normal=mul(Norm, (float3x3)g_mW);
	output.Pos=mul(Pos,g_mWVP);
	//���C�g����
	output.Light=g_vLightDir;
	//�����x�N�g��
	float3 PosWorld = mul(Pos,g_mW);
    output.EyeVector = g_vEye - PosWorld;
	
	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector); 
	float4 NL = saturate(dot(Normal, LightDir)); 
	
	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= g_Diffuse * NL + specular*g_Specular;
	
	//�e�N�X�`���[���W
	output.Tex=Tex;

	return output;
}

//
//�s�N�Z���V�F�[�_�[
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 color=g_texColor.Sample( g_samLinear, input.Tex );
	color+=input.Color/2;

	return color;
}