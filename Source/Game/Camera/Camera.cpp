/**
* @file Camera.h
* @brief Camera
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "Camera.h"
#include "System/Common.h"

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3dx10.lib")

/**
* @fn Camera
* @brief コンストラクタ
*/
Camera::Camera()
{
	
}

/**
* @fn ~Camera
* @brief デストラクタ
*/
Camera::~Camera()
{
}

/**
* @fn SetCameraNumber
* @brief key設定
*/
void Camera::SetCameraNumber(CAMERA_NUMBER key)
{
	m_key = key;
}

// cameraの位置をセット
void Camera::SetCameraPosition(float x, float y, float z)
{
	m_pos = Vector3D(x, y, z);
	SetViewProj();
}

// cameraの位置と注視の位置をセット
void Camera::SetCameraPositionGaze(float x, float y, float z, float gx, float gy, float gz)
{
	m_pos = Vector3D(x, y, z);
	m_gazePos = Vector3D(gx, gy, gz);
	m_isGaze = true;
	SetViewProj();
}

void Camera::SetViewProj()
{
	// ビュー
	if (!m_isGaze)
	{
		D3DXVECTOR3 vEyePt(m_pos.x, m_pos.y, m_pos.z); // カメラ(視点)位置
		D3DXVECTOR3 vLookatPt(m_pos.x, m_pos.y, m_pos.z + 1); // 注視位置
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 上方位置

		D3DXMatrixLookAtLH(&m_matrixView, &vEyePt, &vLookatPt, &vUpVec);
	}
	else
	{
		D3DXVECTOR3 vEyePt(m_pos.x, m_pos.y, m_pos.z); // カメラ(視点)位置
		D3DXVECTOR3 vLookatPt(m_gazePos.x, m_gazePos.y, m_gazePos.z); // 注視位置
		D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f); // 上方位置

		D3DXMatrixLookAtLH(&m_matrixView, &vEyePt, &vLookatPt, &vUpVec);
	}
	m_isGaze = false;

	// プロジェクション
	D3DXMatrixPerspectiveFovLH(&m_matrixProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 1100.0f);

}
