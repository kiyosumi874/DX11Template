
cbuffer global2D
{
    matrix g_W : packoffset(c0); //���[���h�s��
    float4 g_color : packoffset(c4); //�F
    float g_ViewPortWidth : packoffset(c5); //�r���[�|�[�g�i�X�N���[���j���T�C�Y
    float g_ViewPortHeight : packoffset(c6); //�r���[�|�[�g�i�X�N���[���j�c�T�C�Y
};


struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};