/**
* @file Camera.h
* @brief Camera(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Math/Math.h"
#include <d3dx10.h>
#include "CameraStruct.h"
#include "Game/Component/Component.h"
#include "Game/Component/Transform/Transform.h"

// using宣言
using math::Vector3D;

/**
* @class Camera
* @brief Cameraクラス(コンポーネント)
*/
class Camera : public Component
{
public:
	/**
	* @fn Start
	* @brief 生成したときに最初に一回だけ走る関数
	*/
	void Start() override
	{
		m_isGaze = false;
		m_key = CAMERA_NUMBER::CAMERA_NONE;
	}

	/**
	* @fn SetCameraNumber
	* @brief key設定
	*/
	void SetCameraNumber(const CAMERA_NUMBER key);

	/**
	* @fn SetCameraPosition
	* @brief cameraの位置をセット
	* @param[in] x カメラのX座標
	* @param[in] y カメラのY座標
	* @param[in] z カメラのZ座標
	*/
	void SetCameraPosition(float x, float y, float z);

	/**
	* @fn SetCameraPositionGaze
	* @brief cameraの位置と注視の位置をセット
	* @param[in] x カメラのX座標
	* @param[in] y カメラのY座標
	* @param[in] z カメラのZ座標
	* @param[in] gx 注視するX座標
	* @param[in] gy 注視するY座標
	* @param[in] gz 注視するZ座標
	*/
	void SetCameraPositionGaze(float x, float y, float z, float gx, float gy, float gz);

	/**
	* @fn GetCameraData
	* @brief カメラのデータ取得
	* @return CameraData カメラのデータ
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
	* @brief ビュー行列とプロジェクション行列を計算
	*/
	void SetViewProj();

	Vector3D m_pos; // cameraの位置
	Vector3D m_gazePos; // カメラの注視
	bool m_isGaze; // true->注視ありモード
	D3DXMATRIX m_matrixView; // ビュー行列
	D3DXMATRIX m_matrixProj; // プロジェクション行列
	CAMERA_NUMBER m_key; // カメラの識別ID
};