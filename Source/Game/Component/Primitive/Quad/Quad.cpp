/**
* @file Quad.cpp
* @brief �l�p�`��`��(�R���|�[�l���g)
* @author shiihara_kiyosumi
* @date 2022_08_10
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Quad.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Transform.h"
#include "Game/Component/Object.h"
#include <cassert>

/**
* @fn Start
* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
*/
void Quad::Start()
{
	m_transform = m_parent->GetComponent<Transform>();
}

/**
* @fn Draw
* @brief �`��
*/
void Quad::Draw()
{
	SetVertexBuffer();

	DrawCommon();
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�v���~�e�B�u�������_�����O
	Direct3D11::GetDeviceContext()->Draw(4, 0);
}

/**
* @fn Init
* @brief ������(��ΌĂяo����)
* @param[in] pos1 �`�悷��l�p�`�̍��W
* @param[in] pos2 �`�悷��l�p�`�̍��W
* @param[in] pos3 �`�悷��l�p�`�̍��W
* @param[in] pos3 �`�悷��l�p�`�̍��W
*/
void Quad::Init(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, const Vector3D& pos4)
{
	CreateVertexBuffer(pos1, pos2, pos3, pos4);
}

/**
* @fn SetVertexBuffer
* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
* @param[in] pos1 �`�悷��l�p�`�̍��W
* @param[in] pos2 �`�悷��l�p�`�̍��W
* @param[in] pos3 �`�悷��l�p�`�̍��W
* @param[in] pos3 �`�悷��l�p�`�̍��W
*/
void Quad::CreateVertexBuffer(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, const Vector3D& pos4)
{
	if (m_is2D)
	{
		PrimitiveVertex2D vertices[] =
		{
			D3DXVECTOR3(pos1.x, pos1.y, 0.0f),
			D3DXVECTOR3(pos2.x, pos2.y, 0.0f),
			D3DXVECTOR3(pos3.x, pos3.y, 0.0f),
			D3DXVECTOR3(pos4.x, pos4.y, 0.0f),
		};
		assert(SUCCEEDED(D3D11::CreateVertexBuffer<PrimitiveVertex2D>(&m_pVertexBuffer, vertices, 4)));
	}
	else
	{
		PrimitiveVertex vertices[] =
		{
			D3DXVECTOR3(pos1.x, pos1.y, pos1.z),D3DXVECTOR3(0,0,-1),
			D3DXVECTOR3(pos2.x, pos2.y, pos2.z),D3DXVECTOR3(0,0,-1),
			D3DXVECTOR3(pos3.x, pos3.y, pos3.z),D3DXVECTOR3(0,0,-1),
			D3DXVECTOR3(pos4.x, pos4.y, pos4.z),D3DXVECTOR3(0,0,-1),
		};
		assert(SUCCEEDED(D3D11::CreateVertexBuffer<PrimitiveVertex>(&m_pVertexBuffer, vertices, 4)));
	}
	
}

/**
* @fn SetVertexBuffer
* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
*/
void Quad::SetVertexBuffer()
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
