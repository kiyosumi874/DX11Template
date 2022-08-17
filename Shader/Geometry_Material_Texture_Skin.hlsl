//��`
#define MAX_BONE_MATRICES 255

//�O���[�o��
Texture2D g_texColor: register(t0);//�e�N�X�`���[�� ���W�X�^�[t(n)
SamplerState g_samLinear : register(s0);//�T���v���[�̓��W�X�^�[s(n)

cbuffer global_0:register(b0)
{
	float4 g_vLight;  //���C�g�̕����x�N�g��
	float4 g_vEye;//�J�����ʒu
};

cbuffer global_1:register(b1)
{
	matrix g_mW;//���[���h�s��
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��
	float4 g_Ambient=float4(0,0,0,0);//�A���r�G���g��
	float4 g_Diffuse=float4(1,0,0,0); //�g�U����(�F�j
	float4 g_Specular=float4(1,1,1,1);//���ʔ���
};

cbuffer global_bones:register(b2)//�{�[���̃|�[�Y�s�񂪓���
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//�X�L�j���O��̒��_�E�@��������
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���
struct VSSkinIn
{
	float3 Pos	: POSITION;//�ʒu   
	float3 Norm : NORMAL;//���_�@��
	float2 Tex	: TEXCOORD;//�e�N�X�`���[���W
	uint4  Bones : BONE_INDEX;//�{�[���̃C���f�b�N�X
	float4 Weights : BONE_WEIGHT;//�{�[���̏d��
};
//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct PSSkinIn
{
	float4 Pos	: SV_Position;//�ʒu
	float3 Norm : NORMAL;//���_�@��
	float2 Tex	: TEXCOORD;//�e�N�X�`���[���W
	float4 Color : COLOR0;//�ŏI�J���[�i���_�V�F�[�_�[�ɂ����Ắj
};

//
//matrix FetchBoneMatrix( uint iBone )
//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ��@�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
matrix FetchBoneMatrix( uint iBone )
{
	return g_mConstBoneWorld[iBone];
}

//
// Skin SkinVert( VSSkinIn Input )
//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos,1);
	float3 Norm = Input.Norm;
	//�{�[��0
	uint iBone=Input.Bones.x;
	float fWeight=Input.Weights.x;
	matrix m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight * mul(Pos,m);
	Output.Norm+=fWeight * mul(Norm,(float3x3)m);
	//�{�[��1
	iBone=Input.Bones.y;
	fWeight=Input.Weights.y;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//�{�[��2
	iBone=Input.Bones.z;
	fWeight=Input.Weights.z;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);
	//�{�[��3
	iBone=Input.Bones.w;
	fWeight=Input.Weights.w;
	m=FetchBoneMatrix(iBone);
	Output.Pos+=fWeight*mul(Pos,m);
	Output.Norm+=fWeight*mul(Norm,(float3x3)m);

	return Output;
}
//
//PSSkinIn VSSkin(VSSkinIn input )
//�o�[�e�b�N�X�V�F�[�_�[
PSSkinIn VSSkin(VSSkinIn input )
{
	PSSkinIn output;
	Skin vSkinned = SkinVert( input);

	output.Pos = mul( vSkinned.Pos, g_mWVP );
	output.Norm = normalize( mul( vSkinned.Norm, (float3x3)g_mW ) );
	output.Tex = input.Tex;
	float3 LightDir = normalize(g_vLight);
	float3 PosWorld = mul(vSkinned.Pos,g_mW);
	float3 ViewDir = normalize(g_vEye-PosWorld);
	float3 Normal = normalize(output.Norm);
	float4 NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= g_Diffuse * NL + specular*g_Specular;

	return output;
}

//
// float4 PSSkin(PSSkinIn input) : SV_Target
//�s�N�Z���V�F�[�_�[
float4 PSSkin(PSSkinIn input) : SV_Target
{	
	float4 TexDiffuse = g_texColor.Sample( g_samLinear, input.Tex ); 

	return  input.Color/2+TexDiffuse/2;
}