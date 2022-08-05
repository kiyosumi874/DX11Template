/**
* @file main.cpp
* @brief 実行したときにここから始まる
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// ヘッダーファイルのインクルード
#include <Windows.h>
#include "System/Director/Director.h"

/** エントリー関数 */
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	// ディレクター作成
	Director* pDirector = new Director;

	if (!pDirector)
	{
		MessageBox(0, "クラス生成失敗　アプリを終了します", NULL, MB_OK);
		return -1;
	}

	// ディレクター初期化
	if (FAILED(pDirector->Init(hInst)))
	{
		MessageBox(0, "クラス初期化失敗　アプリを終了します", NULL, MB_OK);
		delete pDirector;
		return -1;
	}

	pDirector->Run();

	// ディレクター終了
	delete pDirector;
	return 0;
}