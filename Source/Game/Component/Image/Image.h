/**
* @file Image.h
* @brief Image�̕`��
* @author shiihara_kiyosumi
* @date 2022_08_11
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Transform.h"
#include <D3D11.h>
#include <D3DX10.h>


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
class Image
{
public:
	/**
	* @fn �R���X�g���N�^
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	Image();

	/**
	* @fn �f�X�g���N�^
	* @brief �I������
	*/
	~Image();

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

	// getter
	const Vector3D& GetPos() const
	{
		return m_transform->GetPos();
	}

	const Vector3D& GetRotate() const
	{
		return m_transform->GetRotate();
	}

	const Vector3D& GetScale() const
	{
		return m_transform->GetScale();
	}

	// setter
	void SetPos(const Vector3D& pos)
	{
		m_transform->SetPos(pos);
	}

	void SetRotate(const Vector3D& rotate)
	{
		m_transform->SetRotate(rotate);
	}

	void SetScale(const Vector3D& scale)
	{
		m_transform->SetScale(scale);
	}

private:
	Transform* m_transform;
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
