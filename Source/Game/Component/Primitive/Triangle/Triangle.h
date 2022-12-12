/**
* @file Triangle.h
* @brief �O�p�`��`��(�R���|�[�l���g)
* @author shiihara_kiyosumi
* @date 2022_08_08
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Game/Component/Primitive/Primitive.h"
#include "System/Vector3D.h"

/**
* @class Triangle
* @brief �O�p�`��`�悷��N���X
*/
class Triangle : public Primitive
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

	/**
	* @fn Init
	* @brief ������(��ΌĂяo����)
	* @param[in] pos1 �`�悷��O�p�`�̍��W
	* @param[in] pos2 �`�悷��O�p�`�̍��W
	* @param[in] pos3 �`�悷��O�p�`�̍��W
	*/
	void Init(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3);

private:
	/**
	* @fn CreateVertexBuffer
	* @brief �o�[�e�b�N�X�o�b�t�@�[�쐬
	* @param[in] pos1 �`�悷��O�p�`�̍��W
	* @param[in] pos2 �`�悷��O�p�`�̍��W
	* @param[in] pos3 �`�悷��O�p�`�̍��W
	*/
	void CreateVertexBuffer(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3);

	/**
	* @fn SetVertexBuffer
	* @brief �o�[�e�b�N�X�o�b�t�@�[���Z�b�g
	*/
	void SetVertexBuffer();

};
