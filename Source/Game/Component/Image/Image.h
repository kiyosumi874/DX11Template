/**
* @file Image.h
* @brief Image�̕`��
* @author shiihara_kiyosumi
* @date 2022_08_11
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Game/Component/Component.h"
#include "Game/Component/Transform/Transform.h"
#include "System/Math/Math.h"
#include <D3D11.h>
#include <D3DX10.h>

// using�錾
using math::Vector3D;

/**
* @struct ImageShaderConstBuffer
* @brief ImageShader�p�̃R���X�g�o�b�t�@�[,�V�F�[�_�[���Ɠ����łȂ���΂Ȃ�Ȃ�
*/
struct ImageShaderConstBuffer
{
	alignas(16) D3DXMATRIX world;
	alignas(16) float viewPortWidth;
	alignas(16) float viewPortHeight;
};

/**
* @struct ImageVertex
* @brief ���_�̍\����
*/
struct ImageVertex
{
	D3DXVECTOR3 pos; // �ʒu
	D3DXVECTOR2 uv;  // �e�N�X�`���[���W
};


/**
* @class Image
* @brief Image�N���X
*/
class Image : public Component
{
public:
	/**
	* @fn Start
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	void Start() override;

	/**
	* @fn Update
	* @brief �X�V
	*/
	void Update();

	/**
	* @fn Draw
	* @brief �`��
	*/
	void Draw();

	/**
	* @fn Init
	* @brief ������
	* @param[in] pos1 ���S�̈ʒu
	* @param[in] pos2 �T�C�Y
	* @param[in] texFileName �e�N�X�`���[�̃t�@�C����
	* @param[in] isAlpha �A���t�@�u�����f�B���O���L�����Htrue�ŗL��
	*/
	void Init(const Vector3D& centerPos, const Vector3D& size, const char* texFileName, bool isAlpha, const int ID = -1);

	const int GetID() const
	{
		return m_ID;
	}

private:
	bool m_isAlpha; // �A���t�@�u�����f�B���O���L�����Htrue�ŗL��
	int m_ID;
	ID3D11InputLayout* m_pInputLayout; // �C���v�b�g���C�A�E�g
	ID3D11VertexShader* m_pVertexShader; // �o�[�e�b�N�X�V�F�[�_�[
	ID3D11PixelShader* m_pPixelShader; // �s�N�Z���V�F�[�_�[
	ID3D11Buffer* m_pConstantBuffer; // �萔�o�b�t�@�[
	ID3D11Buffer* m_pVertexBuffer; // �o�[�e�b�N�X�o�b�t�@�[

	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[�i�p�ӂ���̂̓��\�[�X�r���[�����ł����j
};
