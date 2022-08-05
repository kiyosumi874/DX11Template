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
	void SetCameraNumber(const CAMERA_NUMBER key);

	/**
	* @fn SetCameraPosition
	* @brief camera�̈ʒu���Z�b�g
	* @param[in] x �J������X���W
	* @param[in] y �J������Y���W
	* @param[in] z �J������Z���W
	*/
	void SetCameraPosition(float x, float y, float z);

	/**
	* @fn SetCameraPositionGaze
	* @brief camera�̈ʒu�ƒ����̈ʒu���Z�b�g
	* @param[in] x �J������X���W
	* @param[in] y �J������Y���W
	* @param[in] z �J������Z���W
	* @param[in] gx ��������X���W
	* @param[in] gy ��������Y���W
	* @param[in] gz ��������Z���W
	*/
	void SetCameraPositionGaze(float x, float y, float z, float gx, float gy, float gz);

	/**
	* @fn GetCameraData
	* @brief �J�����̃f�[�^�擾
	* @return CameraData �J�����̃f�[�^
	*/
	const CameraData GetCameraData() const
	{
		CameraData data;
		data.matrixView = m_matrixView;
		data.matrixProj = m_matrixProj;
		data.key = m_key;
		return data;
	}

private:
	/**
	* @fn SetViewProj
	* @brief �r���[�s��ƃv���W�F�N�V�����s����v�Z
	*/
	void SetViewProj();

	Vector3D m_pos; // camera�̈ʒu
	Vector3D m_gazePos; // �J�����̒���
	bool m_isGaze; // true->�������胂�[�h
	D3DXMATRIX m_matrixView; // �r���[�s��
	D3DXMATRIX m_matrixProj; // �v���W�F�N�V�����s��
	CAMERA_NUMBER m_key; // �J�����̎���ID
};