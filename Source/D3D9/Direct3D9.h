/**
* @file Direct3D9.h
* @brief Direct3D9����𐮗�����
* @author shiihara_kiyosumi
* @date 2022_08_14
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <d3dx9.h>

// �K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")

/**
* @class Direct3D9
* @brief Direct3D9����𐮗�����N���X(�V���O���g��)
*/
class Direct3D9
{
public:
	/**
	* @fn CreateInstance
	* @brief �����̃C���X�^���X�𐶐�
	*/
	static void CreateInstance()
	{
		if (!m_this)
		{
			m_this = new Direct3D9;
		}
	}

	/**
	* @fn DeleteInstance
	* @brief �����̃C���X�^���X���폜
	*/
	static void DeleteInstance()
	{
		if (m_this)
		{
			delete m_this;
			m_this = nullptr;
		}
	}

	static HRESULT Init();

	/**
	* @fn GetDevice
	* @brief D3D9Device�̎擾
	*/
	static LPDIRECT3DDEVICE9 GetDevice() { return m_this->m_pDevice9; }

private:
	/**
	* @fn Direct3D9
	* @brief �R���X�g���N�^
	*/
	Direct3D9();

	/**
	* @fn ~Direct3D9
	* @brief �f�X�g���N�^
	*/
	~Direct3D9();

	LPDIRECT3D9 m_pD3d9; // D3D9�̃I�u�W�F�N�g
	LPDIRECT3DDEVICE9 m_pDevice9; // D3D9�̃f�o�C�X

	static Direct3D9* m_this; // �ÓI�Ȏ����̃|�C���^
};