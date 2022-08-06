/**
* @file Point.h
* @brief �_��`��(�R���|�[�l���g)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Math/Math.h"
#include "Game/Component/Primitive/Primitive.h"

/**
* @class Point
* @brief �_��`�悷��N���X
*/
class Point : public Primitive
{
public:
	/**
	* @fn Start
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	void Start() override;

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
