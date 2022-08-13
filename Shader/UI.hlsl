//�O���[�o��

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer global
{
    matrix world : packoffset(c0); // ���[���h�s��
    float viewPortWidth : packoffset(c4); //�r���[�|�[�g�i�X�N���[���j���T�C�Y
    float viewPortHeight : packoffset(c5); //�r���[�|�[�g�i�X�N���[���j�c�T�C�Y
};

//�\����
struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 UV : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
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
//�s�N�Z���V�F�[�_�[
float4 PS(PS_INPUT input) : SV_Target
{
    return g_texture.Sample(g_sampler, input.UV);
}