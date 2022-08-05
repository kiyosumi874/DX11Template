/**
* @file Input.cpp
* @brief キーボードやコントローラーの入力
* @details Inputクラスはシングルトンなので注意,コントローラ未実装
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#include "Input.h"
#include <Windows.h>
#include <vector>

// using宣言
using std::vector;

// クラスの静的変数の初期化
Input* Input::m_this = nullptr;

/**
* @fn Input
* @brief コンストラクタ
*/
Input::Input()
{
	// m_keys初期化用の配列
	vector<int> initKeyCode[2];
	
	// キーボードの仮想キーコードをセット
	initKeyCode[KEYBOARD].push_back(VK_DOWN);
	initKeyCode[KEYBOARD].push_back(VK_LEFT);
	initKeyCode[KEYBOARD].push_back(VK_RIGHT);
	initKeyCode[KEYBOARD].push_back(VK_UP);
	initKeyCode[KEYBOARD].push_back(static_cast<int>('X'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('V'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('Z'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('C'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('B'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('N'));
	initKeyCode[KEYBOARD].push_back(VK_ESCAPE);
	initKeyCode[KEYBOARD].push_back(VK_RETURN);

	// コントローラーの仮想キーコードをセット
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_DOWN);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_LEFT);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_RIGHT);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_UP);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_A);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_B);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_X);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_Y);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_L);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_R);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_BACK);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_START);

	// m_keysにコピー
	for (int i = 0; i < BUTTON_ID_MAX; i++)
	{
		m_keys[KEYBOARD][i].keyCode = initKeyCode[KEYBOARD][i];
		m_keys[KEYBOARD][i].pressCount = -1;
		m_keys[CONTROLLER_1P][i].keyCode = initKeyCode[CONTROLLER_1P][i];
		m_keys[CONTROLLER_1P][i].pressCount = -1;
	}
}

/**
* @fn ~Input
* @brief デストラクタ
*/
Input::~Input()
{
	// することないので実装してない
}

/**
* @fn Update
* @brief 更新
* @attention １ループに一回必ず呼び出すこと
*/
void Input::Update()
{
	for (int i = 0; i < BUTTON_ID_MAX; i++)
	{
		// キーボードのアップデート
		// GetAsyncKeyStateは押しているとき最上位ビットが立つ
		if (GetAsyncKeyState(m_this->m_keys[KEYBOARD][i].keyCode) & 0x8000) // 0x8000はshortの最上位ビットが立っていることを表す
		{
			m_this->m_keys[KEYBOARD][i].pressCount = max(++m_this->m_keys[KEYBOARD][i].pressCount, 1);
		}
		else
		{
			m_this->m_keys[KEYBOARD][i].pressCount = min(--m_this->m_keys[KEYBOARD][i].pressCount, 0);
		}

		// コントローラー1Pのアップデート
		/*if (GetJoypadInputState(DX_INPUT_PAD1) & m_input->m_keys[CONTROLLER_1P][i].keyCode)
		{
			m_input->m_keys[CONTROLLER_1P][i].pressCount = max(++m_input->m_keys[CONTROLLER_1P][i].pressCount, 1);
		}
		else
		{
			m_input->m_keys[CONTROLLER_1P][i].pressCount = min(--m_input->m_keys[CONTROLLER_1P][i].pressCount, 0);
		}*/

	}
}
