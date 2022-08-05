/**
* @file CameraStruct.h
* @brief camera�n�̍\����
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <d3dx10.h>

/**
* @enum class CAMERA_NUMBER
* @brief �J�����̎���ID
*/
enum class CAMERA_NUMBER
{
	CAMERA_0,

	CAMERA_NONE
};

/**
* @struct CameraData
* @brief �J�����̃f�[�^
*/
struct CameraData
{
	D3DXMATRIX matrixView;
	D3DXMATRIX matrixProj;
	CAMERA_NUMBER key;
};