#pragma once

/**
* @struct Vector3D
* @brief positionなどに使おう
*/
struct Vector3D
{
	/**
	* @fn Vector3D
	* @brief コンストラクタ
	*/
	Vector3D();

	/**
	* @fn Vector3D
	* @brief コンストラクタ
	* @param[in] x float型である
	* @param[in] y float型である
	* @param[in] z float型である
	*/
	Vector3D(float x, float y, float z);

	/**
	* @fn Init
	* @brief 初期化
	*/
	void Init();

	// 値
	float x;
	float y;
	float z;

	// 演算子のオーバーロード

	// 和差積商
	const Vector3D operator+(const Vector3D& rhs) const;
	const Vector3D operator-(const Vector3D& rhs) const;
	const Vector3D operator*(const Vector3D& rhs) const;
	const Vector3D operator/(const Vector3D& rhs) const;

	// 代入演算
	Vector3D& operator=(const Vector3D& rhs);

	// 自身に対する和差積商
	Vector3D& operator+=(const Vector3D& rhs);
	Vector3D& operator-=(const Vector3D& rhs);
	Vector3D& operator*=(const Vector3D& rhs);
	Vector3D& operator/=(const Vector3D& rhs);

	// 比較演算
	bool operator==(const Vector3D& rhs) const;
	bool operator!=(const Vector3D& rhs) const;

};