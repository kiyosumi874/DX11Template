/**
* @file Primitive.h
* @brief �v���~�e�B�u(�e�N���X)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <D3D11.h>
#include "Game/Component/Component.h"

class Transform;

/**
* @class Primitive
* @brief Point�Ȃǂ̐e�N���X
*/
class Primitive : public Component
{
public:
	/**
	* @fn PrimitiveCom
	* @brief �R���X�g���N�^
	*/
	Primitive();

	/**
	* @fn ~PrimitiveCom
	* @brief �f�X�g���N�^
	*/
	virtual ~Primitive() override;

	Transform* m_transform; // transform

	/**
	* @fn Is2D
	* @brief true��2D�`��
	* @param[in] is2D true��2D�`��
	*/
	void Is2D(const bool is2D)
	{
		m_is2D = is2D;
	}

protected:
	/**
	* @fn DrawCommon
	* @brief Primitive���ʂ̕`��
	*/
	void DrawCommon();
	ID3D11Buffer* m_pVertexBuffer; // �o�[�e�b�N�X�o�b�t�@�[
	bool m_is2D; // true��2D�`�� View��Proj�����������,�E�B���h�E�T�C�Y��VS�ɓn����2D�ɒ��߂���
};
