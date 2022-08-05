/**
* @file CameraStruct.h
* @brief camera系の構造体
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include <d3dx10.h>

/**
* @enum class CAMERA_NUMBER
* @brief カメラの識別ID
*/
enum class CAMERA_NUMBER
{
	CAMERA_0,

	CAMERA_NONE
};

/**
* @struct CameraData
* @brief カメラのデータ
*/
struct CameraData
{
	D3DXMATRIX matrixView;
	D3DXMATRIX matrixProj;
	CAMERA_NUMBER key;
};