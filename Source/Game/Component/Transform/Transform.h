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
		position = Vector3D(0.0f, 0.0f, 0.0f);
		rotation = Vector3D(0.0f, 0.0f, 0.0f);
		scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	Vector3D position; // 座標
	Vector3D rotation; // 回転
	Vector3D scale; // 拡大率
};
