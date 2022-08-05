/**
* @file Primitive.h
* @brief �v���~�e�B�u(�e�N���X)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <D3D11.h>
#include "System/Math/Math.h"

// using�錾
using math::Vector3D;

/**
* @class Primitive
* @brief Point�Ȃǂ̐e�N���X
*/
class Primitive
{
public:
	/**
	* @fn Primitive
	* @brief �R���X�g���N�^
	*/
	Primitive();

	/**
	* @fn ~Primitive
	* @brief �f�X�g���N�^
	*/
	virtual ~Primitive();

	/**
	* @fn Draw
	* @brief �`��
	*/
	virtual void Draw() = 0;

	Vector3D m_pos; // �|�W�V����
protected:
	/**
	* @fn DrawCommon
	* @brief Primitive���ʂ̕`��
	*/
	void DrawCommon();
	ID3D11Buffer* m_pVertexBuffer; // �o�[�e�b�N�X�o�b�t�@�[
	bool m_is2D; // true��2D�`�� View��Proj�����������,�E�B���h�E�T�C�Y��VS�ɓn����2D�ɒ��߂���

};
