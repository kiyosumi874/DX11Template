/**
* @file Window.h
* @brief Window����,WndProc(CALLBACK�֐�)�̊Ǘ�
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <windows.h>

/**
* @class Window
* @brief Window����
* @details 2��ĂԂ�static�ϐ������������̂Ŋ댯!!2�ȏ�̃E�B���h�E���g������������N���X�����ǂ���B
*/
class Window
{
public:
	/**
	* @fn Window
	* @brief �R���X�g���N�^
	*/
	Window();

	/**
	* @fn ~Window
	* @brief �f�X�g���N�^
	*/
	~Window();

	/**
	* @fn Init
	* @brief ������
	* @param[in] hInstance �C���X�^���X�n���h��(�A�v���P�[�V�������̂�\���Ă���u�l�v)
	* @param[in] width �E�B���h�E�̉���
	* @param[in] height �E�B���h�E�̏c��
	* @param[in] windowName �E�B���h�E�̖��O
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT Init(HINSTANCE hInstance, INT width, INT height, LPCSTR windowName);

	/**
	* @fn MsgProc
	* @brief ���b�Z�[�W�v���V�[�W��
	* @param[in] hWnd �E�B���h�E�n���h��(�J�����g�E�B���h�E�Ɋ��蓖�Ă�ꂽ�n���h��)
	* @param[in] msg ���b�Z�[�W(�X���b�h�̃��b�Z�[�W�L���[����̃��b�Z�[�W����ێ�����\����)
	* @param[in] wParam ���16bit���ʒm�R�[�h,����16bit���R���g���[��/���j���[ID(�T�C�g�ɏ����Ă������Ȃ̂Ŋm�؂͖���)
	* @param[in] lParam �E�B���h�E�n���h��(����Ɋւ��Ă͂悭�킩���!)
	* @return LRESULT �E�B���h�E�v���V�[�W���Ŏg���߂�l
	*/
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	* @fn GetWindowInstance
	* @brief �ÓI�ȃE�B���h�E�C���X�^���X�𓾂�
	* @return static Window* �ÓI�m�ۂ��������̃C���X�^���X
	* @details WndProc��MsgProc���ĂԂ��߂Ɏ����̃C���X�^���X��ÓI�Ɋm�ۂ��Ă���(���I�Ɋm�ۂ������̂��Ɩ���������)
	*/
	static Window* GetWindowInstance() { return m_windowInstance; }

	/**
	* @fn GetWindowHandle
	* @brief �E�B���h�E�n���h���𓾂�
	* @return static HWND �E�B���h�E�n���h��
	*/
	static HWND GetWindowHandle() { return m_windowInstance->m_hWnd; }
private:
	static Window* m_windowInstance; // �ÓI�Ȏ����̃C���X�^���X

	HWND m_hWnd; // �E�B���h�E�n���h��
};
