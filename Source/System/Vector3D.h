#pragma once

/**
* @struct Vector3D
* @brief position�ȂǂɎg����
*/
struct Vector3D
{
	/**
	* @fn Vector3D
	* @brief �R���X�g���N�^
	*/
	Vector3D();

	/**
	* @fn Vector3D
	* @brief �R���X�g���N�^
	* @param[in] x float�^�ł���
	* @param[in] y float�^�ł���
	* @param[in] z float�^�ł���
	*/
	Vector3D(float x, float y, float z);

	/**
	* @fn Init
	* @brief ������
	*/
	void Init();

	// �l
	float x;
	float y;
	float z;

	// ���Z�q�̃I�[�o�[���[�h

	// �a���Ϗ�
	const Vector3D operator+(const Vector3D& rhs) const;
	const Vector3D operator-(const Vector3D& rhs) const;
	const Vector3D operator*(const Vector3D& rhs) const;
	const Vector3D operator/(const Vector3D& rhs) const;

	// ������Z
	Vector3D& operator=(const Vector3D& rhs);

	// ���g�ɑ΂���a���Ϗ�
	Vector3D& operator+=(const Vector3D& rhs);
	Vector3D& operator-=(const Vector3D& rhs);
	Vector3D& operator*=(const Vector3D& rhs);
	Vector3D& operator/=(const Vector3D& rhs);

	// ��r���Z
	bool operator==(const Vector3D& rhs) const;
	bool operator!=(const Vector3D& rhs) const;

};