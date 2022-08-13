/**
* @file Camera.h
* @brief Camera(�R���|�[�l���g)
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "System/Math/Math.h"
#include <d3dx10.h>
#include "CameraStruct.h"
#include "Game/Component/Component.h"
#include "Game/Component/Transform/Transform.h"

// using�錾
using math::Vector3D;

/**
* @class Camera
* @brief Camera�N���X(�R���|�[�l���g)
*/
class Camera : public Component
{
public:
	/**
	* @fn Start
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	void Start() override
	{
		m_isGaze = false;
		m_key = CAMERA_NUMBER::CAMERA_NONE;
	}

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
		data.pos = m_pos;
		data.gazePos = m_gazePos;
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