/**
* @file Line.cpp
* @brief ����`��(�R���|�[�l���g)
* @author shiihara_kiyosumi
* @date 2022_08_08
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Line.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Transform.h"
#include "Game/Component/Object.h"

/**
* @fn Start
* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
*/
void Line::Start()
{
	m_transform = m_parent->GetComponent<Transform>();
}

/**
* @fn Draw
* @brief �`��
*/
void Line::Draw()
{
	SetVertexBuffer();
	
	DrawCommon();
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	//�v���~�e�B�u�������_�����O
	Direct3D11::GetDeviceContext()->Draw(2, 0);
}

/**
* @fn Init
* @brief ������(��ΌĂяo����)
* @param[in] pos1 �`�悷����̋N�_���W
* @param[in] pos2 �`�悷����̏I�_���W
*/
void Line::Init(const Vector3D& pos1, const Vector3D& pos2)
{
	CreateVertexBuffer(pos1, pos2);
}

/**
* @fn SetVertexBuffer
* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
* @param[in] pos1 �`�悷����̋N�_���W
* @param[in] pos2 �`�悷����̏I�_���W
*/
void Line::CreateVertexBuffer(const Vector3D& pos1, const Vector3D& pos2)
{
	
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	if (m_is2D)
	{
		bd.ByteWidth = sizeof(PrimitiveVertex2D) * 2;
		PrimitiveVertex2D vertices[] =
		{
			D3DXVECTOR3(pos1.x, pos1.y, pos1.z),
			D3DXVECTOR3(pos2.x, pos2.y, pos2.z),
		};
		InitData.pSysMem = vertices;
	}
	else
	{
		bd.ByteWidth = sizeof(PrimitiveVertex) * 2;
		PrimitiveVertex vertices[] =
		{
			D3DXVECTOR3(pos1.x, pos1.y, pos1.z),D3DXVECTOR3(0,0,1),
			D3DXVECTOR3(pos2.x, pos2.y, pos2.z),D3DXVECTOR3(0,0,1),
		};
		InitData.pSysMem = vertices;
	}

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		// �G���[���b�Z�[�W
	}
}

/**
* @fn SetVertexBuffer
* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
*/
void Line::SetVertexBuffer()
{
	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	if (m_is2D)
	{
		UINT stride = sizeof(PrimitiveVertex2D);
		UINT offset = 0;
		Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}
	else
	{
		UINT stride = sizeof(PrimitiveVertex);
		UINT offset = 0;
		Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}
}
