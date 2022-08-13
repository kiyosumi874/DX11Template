/**
* @file Transform.h
* @brief �ʒu,��],�g�嗦�̏��(�R���|�[�l���g)
* @author shiihara_kiyosumi
* @date 2022_08_07
*/


// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Math/Math.h"
#include "Game/Component/Component.h"

// using�錾
using math::Vector3D;

/**
* @class Transform
* @brief �ʒu,��],�g�嗦�̏��N���X(�R���|�[�l���g)
*/
class Transform : public Component
{
public:
	/**
	* @fn Start
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	void Start() override
	{
		position = Vector3D(0.0f, 0.0f, 0.0f);
		rotation = Vector3D(0.0f, 0.0f, 0.0f);
		scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	Vector3D position; // ���W
	Vector3D rotation; // ��]
	Vector3D scale; // �g�嗦
};
