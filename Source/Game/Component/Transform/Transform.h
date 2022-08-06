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
		m_position = Vector3D(0.0f, 0.0f, 0.0f);
		m_rotation = Vector3D(0.0f, 0.0f, 0.0f);
		m_scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	Vector3D m_position; // ���W
	Vector3D m_rotation; // ��]
	Vector3D m_scale; // �g�嗦
};
