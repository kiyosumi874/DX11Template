/**
* @file Window.cpp
* @brief Window生成,WndProc(CALLBACK関数)の管理
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// ヘッダーファイルのインクルード
#include "Window.h"
#include "imgui.h"

// 静的変数の初期化
Window* Window::m_windowInstance = nullptr;

// extern宣言
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//	ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam)) { return true; }

	return Window::GetWindowInstance()->MsgProc(hWnd, uMsg, wParam, lParam);
}

/**
* @fn Window
* @brief コンストラクタ
*/
Window::Window()
{
	// 変数がすべて初期化される
	ZeroMemory(this, sizeof(Window));
	// 自分のインスタンスを静的変数にコピー
	m_windowInstance = this;
}

/**
* @fn ~Window
* @brief デストラクタ
*/
Window::~Window()
{
	// 解放処理
	m_windowInstance = nullptr;
}

/**
* @fn Init
* @brief 初期化
* @param[in] hInstance インスタンスハンドル(アプリケーション自体を表している「値」)
* @param[in] width ウィンドウの横幅
* @param[in] height ウィンドウの縦幅
* @param[in] windowName ウィンドウの名前
* @return HRESULT S_OKで成功
*/
HRESULT Window::Init(HINSTANCE hInstance, INT width, INT height, LPCSTR windowName)
{
	// ウィンドウの定義
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

	//ウィンドウの作成
	m_hWnd = CreateWindow(windowName, windowName, WS_OVERLAPPEDWINDOW,
		0, 0, width, height, 0, 0, hInstance, 0);

	// エラーチェック
	if (!m_hWnd) { return E_FAIL; }

	//ウインドウの表示
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}

/**
* @fn MsgProc
* @brief メッセージプロシージャ
* @param[in] hWnd ウィンドウハンドル(カレントウィンドウに割り当てられたハンドル)
* @param[in] msg メッセージ(スレッドのメッセージキューからのメッセージ情報を保持する構造体)
* @param[in] wParam 上位16bitが通知コード,下位16bitがコントロール/メニューID(サイトに書いてただけなので確証は無い)
* @param[in] lParam ウィンドウハンドル(これに関してはよくわからん!)
* @return LRESULT ウィンドウプロシージャで使う戻り値
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
