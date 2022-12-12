/**
* @file Transform.h
* @brief 位置,回転,拡大率の情報
* @author shiihara_kiyosumi
* @date 2022_08_07
*/


// ヘッダーファイルのインクルード
#pragma once
#include "System/Vector3D.h"

/**
* @class Transform
* @brief 位置,回転,拡大率の情報クラス
*/
class Transform
{
public:
	Transform()
	{
		position = Vector3D(0.0f, 0.0f, 0.0f);
		rotation = Vector3D(0.0f, 0.0f, 0.0f);
		scale = Vector3D(1.0f, 1.0f, 1.0f);
	}

	// getter
	const Vector3D& GetPos() const
	{
		return position;
	}

	const Vector3D& GetRotate() const
	{
		return rotation;
	}

	const Vector3D& GetScale() const
	{
		return scale;
	}

	// setter
	void SetPos(const Vector3D& pos)
	{
		position = pos;
	}

	void SetRotate(const Vector3D& rotate)
	{
		rotation = rotate;
	}

	void SetScale(const Vector3D& scale)
	{
		this->scale = scale;
	}

private:
	Vector3D position; // 座標
	Vector3D rotation; // 回転
	Vector3D scale; // 拡大率
};
