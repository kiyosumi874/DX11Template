/**
* @file Camera.h
* @brief Camera
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Math/Math.h"
#include <d3dx10.h>
#include "CameraStruct.h"

// using宣言
using math::Vector3D;

/**
* @class Camera
* @brief Cameraクラス
*/
class Camera
{
public:
	/**
	* @fn Camera
	* @brief コンストラクタ
	*/
	Camera();

	/**
	* @fn ~Camera
	* @brief デストラクタ
	*/
	~Camera();

	/**
	* @fn SetCameraNumber
	* @brief key設定
	*/
	void SetCameraNumber(CAMERA_NUMBER key);

	// cameraの位置をセット
	void SetCameraPosition(float x, float y, float z);

	// cameraの位置と注視の位置をセット
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

	Vector3D m_pos; // cameraの位置
	Vector3D m_gazePos; // カメラの注視
	bool m_isGaze; // true->注視ありモード
	D3DXMATRIX m_matrixView;
	D3DXMATRIX m_matrixProj;
	CAMERA_NUMBER m_key;
};