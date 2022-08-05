/**
* @file Camera.h
* @brief Camera
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Math/Math.h"
#include <d3dx10.h>
#include "CameraStruct.h"

// using�錾
using math::Vector3D;

/**
* @class Camera
* @brief Camera�N���X
*/
class Camera
{
public:
	/**
	* @fn Camera
	* @brief �R���X�g���N�^
	*/
	Camera();

	/**
	* @fn ~Camera
	* @brief �f�X�g���N�^
	*/
	~Camera();

	/**
	* @fn SetCameraNumber
	* @brief key�ݒ�
	*/
	void SetCameraNumber(CAMERA_NUMBER key);

	// camera�̈ʒu���Z�b�g
	void SetCameraPosition(float x, float y, float z);

	// camera�̈ʒu�ƒ����̈ʒu���Z�b�g
	void SetCameraPositionGaze(float x, float y, float z, float gx, float gy, float gz);

	const CameraData GetCameraData() const
	{
		CameraData data;
		data.matrixView = m_matrixView;
		data.matrixProj = m_matrixProj;
		data.key = m_key;
		return data;
	}

private:
	void SetViewProj();

	Vector3D m_pos; // camera�̈ʒu
	Vector3D m_gazePos; // �J�����̒���
	bool m_isGaze; // true->�������胂�[�h
	D3DXMATRIX m_matrixView;
	D3DXMATRIX m_matrixProj;
	CAMERA_NUMBER m_key;
};