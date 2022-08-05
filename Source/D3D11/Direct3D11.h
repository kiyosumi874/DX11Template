/**
* @file Direct3D11.h
* @brief Direct3D11����𐮗�����
* @author shiihara_kiyosumi
* @date 2022_07_29
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <Windows.h>
#include <D3D11.h>
	
/**
* @class Direct3D11
* @brief Direct3D11����𐮗�����N���X(�V���O���g��)
*/
class Direct3D11
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
			m_this = new Direct3D11;
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

	/**
	* @fn Init
	* @brief ������
	* @param[in] hWnd �E�B���h�E�n���h��(�J�����g�E�B���h�E�Ɋ��蓖�Ă�ꂽ�n���h��)
	* @return HRESULT S_OK�Ő���
	*/
	static HRESULT Init(HWND hWnd);

	/**
	* @fn Clear
	* @brief �P�F�ŉ�ʂ�h��Ԃ�
	*/
	static void Clear(int red = 255, int green = 255, int blue = 255);

	/**
	* @fn Present
	* @brief ��ʍX�V
	*/
	static HRESULT Present();

	/**
	* @fn GetDevice
	* @brief D3D11Device�̎擾
	*/
	static ID3D11Device* GetDevice() { return m_this->m_pDevice; }

	/**
	* @fn GetDeviceContext
	* @brief D3D11DeviceContext�̎擾
	*/
	static ID3D11DeviceContext* GetDeviceContext() { return m_this->m_pDeviceContext; }

private:
	/**
	* @fn Direct3D11
	* @brief �R���X�g���N�^
	*/
	Direct3D11();

	/**
	* @fn Direct3D11
	* @brief �f�X�g���N�^
	*/
	~Direct3D11();

	ID3D11Device* m_pDevice; // <�r�f�I�J�[�h>�Ƃ��������I�f�o�C�X�����f������������
	ID3D11DeviceContext* m_pDeviceContext; // GraphicsDeviceInterface(GDI)���Ǘ�����f�[�^�\���ł���o�̓f�o�C�X(�f�B�X�v���C�Ȃ�)�̑������i�[�����
	IDXGISwapChain* m_pSwapChain; // <���>�����f���������I�u�W�F�N�g(�t�����g�o�b�t�@�ƈ�ȏ�̃o�b�N�o�b�t�@�̂���)

	// �����ɂ���r���[�́u���\�[�X�ƃ����_�����O�p�C�v���C�����֘A�t����I�u�W�F�N�g�v�̂���
	// �P�Ȃ�o�C�g�̉�ł��郊�\�[�X���g���ɂ̓r���[���K�v�ɂȂ�
	// �r���[��C����ɂ�����^�L���X�g�̂悤�Ȃ���
	ID3D11RenderTargetView* m_pRenderTargetView; // �����_�[�^�[�Q�b�g�ƃ��\�[�X���Ȃ��鋴�̂悤�Ȃ���
	ID3D11DepthStencilView* m_pDepthStencilView; // �f�v�X�X�e���V���ƃ��\�[�X���Ȃ��鋴�̂悤�Ȃ���
	
	// DepthBuffer�̓s�N�Z�����̐[�x�����L�^���Ă������̂ŁA����ɂ�蕨�̂̑O��֌W�𐳂����\�������鎖���o����
	// StencilBuffer�̓I���^�C���Ń}�X�N�̕ύX�����R�ɂł���
	ID3D11Texture2D* m_pDepthStencilBuffer; // �񎟌��I�ȃo�b�t�@�[�Ȃ̂�Texture2D.���O�ɘf�킳����

	ID3D11DepthStencilState* m_pDepthStencilState; // depthStencil�̐ݒ�
	ID3D11BlendState* m_pBlendState; // �u�����h�̐ݒ�
	ID3D11RasterizerState* m_pRasterizerState; // ���X�^���C�U�[�̐ݒ�

	static Direct3D11* m_this; // �ÓI�Ȏ����̃|�C���^
};