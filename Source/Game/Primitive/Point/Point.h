/**
* @file Point.h
* @brief �_��`��
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Math/Math.h"
#include "Game/Primitive/Primitive.h"

// using�錾
using math::Vector3D;

/**
* @class Point
* @brief �_��`�悷��N���X
*/
class Point : public Primitive
{
public:
	/**
	* @fn Point
	* @brief �R���X�g���N�^
	*/
	Point();

	/**
	* @fn Point
	* @brief �����t���R���X�g���N�^
	* @param[in] pos ���W
	* @param[in] is2D true��2D�`��
	*/
	Point(const Vector3D& pos, bool is2D = false);

	/**
	* @fn ~Point
	* @brief �f�X�g���N�^
	*/
	~Point() override;

	/**
	* @fn Draw
	* @brief �`��
	*/
	void Draw() override;

private:
	/**
	* @fn CreateVertexBuffer
	* @brief �o�[�e�b�N�X�o�b�t�@�[�쐬
	*/
	void CreateVertexBuffer();

	/**
	* @fn SetVertexBuffer
	* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	*/
	void SetVertexBuffer();

};
