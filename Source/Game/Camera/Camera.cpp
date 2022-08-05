/**
* @file Camera.h
* @brief Camera
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Camera.h"
#include "System/Common.h"

// �K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"d3dx10.lib")

/**
* @fn Camera
* @brief �R���X�g���N�^
*/
Camera::Camera()
{
	
}

/**
* @fn ~Camera
* @brief �f�X�g���N�^
*/
Camera::~Camera()
{
}

/**
* @fn SetCameraNumber
* @brief key�ݒ�
*/
void Camera::SetCameraNumber(CAMERA_NUMBER key)
{
	m_key = key;
}

// camera�̈ʒu���Z�b�g
void Camera::SetCameraPosition(float x, float y, float z)
{
	m_pos = Vector3D(x, y, z);
	SetViewProj();
}

// camera�̈ʒu�ƒ����̈ʒu���Z�b�g
void Camera::SetCameraPositionGaze(float x, float y, float z, float gx, float gy, float gz)
{
	m_pos = Vector3D(x, y, z);
	m_gazePos = Vector3D(gx, gy, gz);
	m_isGaze = true;
	SetViewProj();
}

void Camera::SetViewProj()
{
	// �r���[
	if (!m_isGaze)
	{
		D3DXVECTOR3 vEyePt(m_pos.x, m_pos.y, m_pos.z); // �J����(���_)�ʒu
		D3DXVECTOR3 vLookatPt(m_pos.x, m_pos.y, m_pos.z + 1); // �����ʒu
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // ����ʒu

		D3DXMatrixLookAtLH(&m_matrixView, &vEyePt, &vLookatPt, &vUpVec);
	}
	else
	{
		D3DXVECTOR3 vEyePt(m_pos.x, m_pos.y, m_pos.z); // �J����(���_)�ʒu
		D3DXVECTOR3 vLookatPt(m_gazePos.x, m_gazePos.y, m_gazePos.z); // �����ʒu
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // ����ʒu

		D3DXMatrixLookAtLH(&m_matrixView, &vEyePt, &vLookatPt, &vUpVec);
	}
	m_isGaze = false;

	// �v���W�F�N�V����
	D3DXMatrixPerspectiveFovLH(&m_matrixProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1100.0f);

}
