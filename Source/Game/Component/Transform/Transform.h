/**
* @file Transform.h
* @brief 位置,回転,拡大率の情報(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_07
*/


// ヘッダーファイルのインクルード
#pragma once
#include "System/Math/Math.h"
#include "Game/Component/Component.h"

// using宣言
using math::Vector3D;

/**
* @class Transform
* @brief 位置,回転,拡大率の情報クラス(コンポーネント)
*/
class Transform : public Component
{
public:
	/**
	* @fn Start
	* @brief 生成したときに最初に一回だけ走る関数
	*/
	void Start() override
	{
		m_position = Vector3D(0.0f, 0.0f, 0.0f);
		m_rotation = Vector3D(0.0f, 0.0f, 0.0f);
		m_scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	Vector3D m_position; // 座標
	Vector3D m_rotation; // 回転
	Vector3D m_scale; // 拡大率
};
