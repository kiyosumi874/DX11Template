/**
* @file Math.cpp
* @brief 算術系便利ファイル
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "Math.h"

// using宣言
using math::Vector3D;

/**
 * @fn Vector3D
 * @brief コンストラクタ
 */
Vector3D::Vector3D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

/**
* @fn Vector3D
* @brief コンストラクタ
* @param[in] x float型である
* @param[in] y float型である
* @param[in] z float型である
*/
Vector3D::Vector3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

/**
* @fn Init
* @brief 初期化
*/
void Vector3D::Init()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

// 和
const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
    return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

// 差
const Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
    return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

// 積
const Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
    return Vector3D(x * rhs.x, y * rhs.y, z * rhs.z);
}

// 商
const Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
    return Vector3D(x / rhs.x, y / rhs.y, z / rhs.z);
}

// 代入演算
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

// 自身に対する和
Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// 自身に対する差
Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

// 自身に対する積
Vector3D& Vector3D::operator*=(const Vector3D& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

// 自身に対する商
Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

// 比較演算
bool Vector3D::operator==(const Vector3D& rhs) const
{
    return (x == rhs.x && y == rhs.y && z == rhs.z);
}

// 比較演算
bool Vector3D::operator!=(const Vector3D& rhs) const
{
    return !(x == rhs.x && y == rhs.y && z == rhs.z);
}
