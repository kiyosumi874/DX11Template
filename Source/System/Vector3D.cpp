#include "Vector3D.h"

/**
 * @fn Vector3D
 * @brief �R���X�g���N�^
 */
Vector3D::Vector3D()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

/**
* @fn Vector3D
* @brief �R���X�g���N�^
* @param[in] x float�^�ł���
* @param[in] y float�^�ł���
* @param[in] z float�^�ł���
*/
Vector3D::Vector3D(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

/**
* @fn Init
* @brief ������
*/
void Vector3D::Init()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

// �a
const Vector3D Vector3D::operator+(const Vector3D& rhs) const
{
    return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
}

// ��
const Vector3D Vector3D::operator-(const Vector3D& rhs) const
{
    return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
}

// ��
const Vector3D Vector3D::operator*(const Vector3D& rhs) const
{
    return Vector3D(x * rhs.x, y * rhs.y, z * rhs.z);
}

// ��
const Vector3D Vector3D::operator/(const Vector3D& rhs) const
{
    return Vector3D(x / rhs.x, y / rhs.y, z / rhs.z);
}

// ������Z
Vector3D& Vector3D::operator=(const Vector3D& rhs)
{
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
}

// ���g�ɑ΂���a
Vector3D& Vector3D::operator+=(const Vector3D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
}

// ���g�ɑ΂��鍷
Vector3D& Vector3D::operator-=(const Vector3D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
}

// ���g�ɑ΂����
Vector3D& Vector3D::operator*=(const Vector3D& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    return *this;
}

// ���g�ɑ΂��鏤
Vector3D& Vector3D::operator/=(const Vector3D& rhs)
{
    x /= rhs.x;
    y /= rhs.y;
    z /= rhs.z;
    return *this;
}

// ��r���Z
bool Vector3D::operator==(const Vector3D& rhs) const
{
    return (x == rhs.x && y == rhs.y && z == rhs.z);
}

// ��r���Z
bool Vector3D::operator!=(const Vector3D& rhs) const
{
    return !(x == rhs.x && y == rhs.y && z == rhs.z);
}