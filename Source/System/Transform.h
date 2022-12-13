/**
* @file Transform.h
* @brief �ʒu,��],�g�嗦�̏��
* @author shiihara_kiyosumi
* @date 2022_08_07
*/


// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Vector3D.h"

/**
* @class Transform
* @brief �ʒu,��],�g�嗦�̏��N���X
*/
class Transform
{
public:
	Transform()
	{
		position = Vector3D(0.0f, 0.0f, 0.0f);
		rotation = Vector3D(0.0f, 0.0f, 0.0f);
		scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	Transform(const Vector3D& pos, const Vector3D& rot, const Vector3D& scale)
	{
		position = pos;
		rotation = rot;
		this->scale = scale;
	}

	// getter
	const Vector3D& GetPos() const
	{
		return position;
	}

	const Vector3D& GetRotate() const
	{
		return rotation;
	}

	const Vector3D& GetScale() const
	{
		return scale;
	}

	// setter
	void SetPos(const Vector3D& pos)
	{
		position = pos;
	}

	void SetRotate(const Vector3D& rotate)
	{
		rotation = rotate;
	}

	void SetScale(const Vector3D& scale)
	{
		this->scale = scale;
	}

private:
	Vector3D position; // ���W
	Vector3D rotation; // ��]
	Vector3D scale; // �g�嗦
};
