/**
* @file Point.cpp
* @brief �_��`��
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Point.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"

/**
* @fn Point
* @brief �R���X�g���N�^
*/
Point::Point()
{
	m_is2D = false;
	CreateVertexBuffer();
}

/**
* @fn Point
* @brief �����t���R���X�g���N�^
* @param[in] pos ���W
* @param[in] is2D true��2D�`��
*/
Point::Point(const Vector3D& pos, bool is2D)
{
	m_is2D = is2D;
	m_pos = pos;
	CreateVertexBuffer();
}

/**
* @fn ~Point
* @brief �f�X�g���N�^
*/
Point::~Point()
{
	// ������
}

/**
* @fn Draw
* @brief �`��
*/
void Point::Draw()
{
	SetVertexBuffer();

	DrawCommon();
}

/**
* @fn CreateVertexBuffer
* @brief �o�[�e�b�N�X�o�b�t�@�[�쐬
*/
void Point::CreateVertexBuffer()
{
	PrimitiveVertex vertices[] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PrimitiveVertex) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		// �G���[���b�Z�[�W
	}
}

/**
* @fn SetVertexBuffer
* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
*/
void Point::SetVertexBuffer()
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	UINT stride = sizeof(PrimitiveVertex);
	UINT offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
}
