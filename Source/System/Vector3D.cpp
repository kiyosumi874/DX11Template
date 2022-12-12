#include "Vector3D.h"

/**
 * @fn Vector3D
 * @brief ƒRƒ“ƒXƒgƒ‰ƒNƒ^
 */
Vector3D::Vector3D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

/**
* @fn Vector3D
* @brief ƒRƒ“ƒXƒgƒ‰ƒNƒ^
* @param[in] x floatŒ^‚Å‚ ‚é
* @param[in] y floatŒ^‚Å‚ ‚é
* @param[in] z floatŒ^‚Å‚ ‚é
*/
Vector3D::Vector3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

/**
* @fn Init
* @brief ‰Šú‰»
*/
void Vector3D::Init()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

// ˜a
const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
    return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

// ·
const Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
    return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

// Ï
const Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
    return Vector3D(x * rhs.x, y * rhs.y, z * rhs.z);
}

// ¤
const Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
    return Vector3D(x / rhs.x, y / rhs.y, z / rhs.z);
}

// ‘ã“ü‰‰Z
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

// ©g‚É‘Î‚·‚é˜a
Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// ©g‚É‘Î‚·‚é·
Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

// ©g‚É‘Î‚·‚éÏ
Vector3D& Vector3D::operator*=(const Vector3D& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

// ©g‚É‘Î‚·‚é¤
Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

// ”äŠr‰‰Z
bool Vector3D::operator==(const Vector3D& rhs) const
{
    return (x == rhs.x && y == rhs.y && z == rhs.z);
}

// ”äŠr‰‰Z
bool Vector3D::operator!=(const Vector3D& rhs) const
{
    return !(x == rhs.x && y == rhs.y && z == rhs.z);
}