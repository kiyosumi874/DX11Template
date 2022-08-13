/**
* @file Window.cpp
* @brief Window����,WndProc(CALLBACK�֐�)�̊Ǘ�
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Window.h"
#include "imgui.h"

// �ÓI�ϐ��̏�����
Window* Window::m_windowInstance = nullptr;

// extern�錾
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//	�E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) { return true; }

	return Window::GetWindowInstance()->MsgProc(hWnd, uMsg, wParam, lParam);
}

/**
* @fn Window
* @brief �R���X�g���N�^
*/
Window::Window()
{
	// �ϐ������ׂď����������
	ZeroMemory(this, sizeof(Window));
	// �����̃C���X�^���X��ÓI�ϐ��ɃR�s�[
	m_windowInstance = this;
}

/**
* @fn ~Window
* @brief �f�X�g���N�^
*/
Window::~Window()
{
	// �������
	m_windowInstance = nullptr;
}

/**
* @fn Init
* @brief ������
* @param[in] hInstance �C���X�^���X�n���h��(�A�v���P�[�V�������̂�\���Ă���u�l�v)
* @param[in] width �E�B���h�E�̉���
* @param[in] height �E�B���h�E�̏c��
* @param[in] windowName �E�B���h�E�̖��O
* @return HRESULT S_OK�Ő���
*/
HRESULT Window::Init(HINSTANCE hInstance, INT width, INT height, LPCSTR windowName)
{
	// �E�B���h�E�̒�`
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = windowName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wc);

	//�E�B���h�E�̍쐬
	m_hWnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, 0, 0, hInstance, 0);

	// �G���[�`�F�b�N
	if (!m_hWnd) { return E_FAIL; }

	//�E�C���h�E�̕\��
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

/**
* @fn MsgProc
* @brief ���b�Z�[�W�v���V�[�W��
* @param[in] hWnd �E�B���h�E�n���h��(�J�����g�E�B���h�E�Ɋ��蓖�Ă�ꂽ�n���h��)
* @param[in] msg ���b�Z�[�W(�X���b�h�̃��b�Z�[�W�L���[����̃��b�Z�[�W����ێ�����\����)
* @param[in] wParam ���16bit���ʒm�R�[�h,����16bit���R���g���[��/���j���[ID(�T�C�g�ɏ����Ă������Ȃ̂Ŋm�؂͖���)
* @param[in] lParam �E�B���h�E�n���h��(����Ɋւ��Ă͂悭�킩���!)
* @return LRESULT �E�B���h�E�v���V�[�W���Ŏg���߂�l
*/
LRESULT Window::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
