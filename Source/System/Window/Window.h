/**
* @file Window.h
* @brief Window生成,WndProc(CALLBACK関数)の管理
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// ヘッダーファイルのインクルード
#pragma once
#include <windows.h>

/**
* @class Window
* @brief Window生成
* @details 2回呼ぶとstatic変数が書き換わるので危険!!2つ以上のウィンドウを使う時が来たらクラスを改良する。
*/
class Window
{
public:
	/**
	* @fn Window
	* @brief コンストラクタ
	*/
	Window();

	/**
	* @fn ~Window
	* @brief デストラクタ
	*/
	~Window();

	/**
	* @fn Init
	* @brief 初期化
	* @param[in] hInstance インスタンスハンドル(アプリケーション自体を表している「値」)
	* @param[in] width ウィンドウの横幅
	* @param[in] height ウィンドウの縦幅
	* @param[in] windowName ウィンドウの名前
	* @return HRESULT S_OKで成功
	*/
	HRESULT Init(HINSTANCE hInstance, INT width, INT height, LPCSTR windowName);

	/**
	* @fn MsgProc
	* @brief メッセージプロシージャ
	* @param[in] hWnd ウィンドウハンドル(カレントウィンドウに割り当てられたハンドル)
	* @param[in] msg メッセージ(スレッドのメッセージキューからのメッセージ情報を保持する構造体)
	* @param[in] wParam 上位16bitが通知コード,下位16bitがコントロール/メニューID(サイトに書いてただけなので確証は無い)
	* @param[in] lParam ウィンドウハンドル(これに関してはよくわからん!)
	* @return LRESULT ウィンドウプロシージャで使う戻り値
	*/
	LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	/**
	* @fn GetWindowInstance
	* @brief 静的なウィンドウインスタンスを得る
	* @return static Window* 静的確保した自分のインスタンス
	* @details WndProcでMsgProcを呼ぶために自分のインスタンスを静的に確保してある(動的に確保したものだと無理だから)
	*/
	static Window* GetWindowInstance() { return m_windowInstance; }

	/**
	* @fn GetWindowHandle
	* @brief ウィンドウハンドルを得る
	* @return static HWND ウィンドウハンドル
	*/
	static HWND GetWindowHandle() { return m_windowInstance->m_hWnd; }
private:
	static Window* m_windowInstance; // 静的な自分のインスタンス

	HWND m_hWnd; // ウィンドウハンドル
};
