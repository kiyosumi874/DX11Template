/**
* @file Director.h
* @brief プログラムの管理
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// ヘッダーファイルのインクルード
#pragma once
#include <Windows.h>
#include "System/Window/Window.h"
#include "Scene/SceneManager/SceneManager.h"

/**
* @class Director
* @brief プログラムの管理クラス
*/
class Director
{
public:
	/**
	* @fn Director
	* @brief コンストラクタ
	*/
	Director();

	/**
	* @fn ~Director
	* @brief デストラクタ
	*/
	~Director();

	/**
	* @fn Init
	* @brief 初期化
	* @param[in] hInstance インスタンスハンドル(アプリケーション自体を表している「値」)
	* @return HRESULT S_OKで成功
	*/
	HRESULT Init(HINSTANCE hInstance);

	/**
	* @fn Run
	* @brief 実行
	*/
	void Run();

private:
	// trueで終了,falseで何もない
	bool MainLoop();

	Window* m_pWindow; // ウィンドウクラスのポインタ
	SceneManager* m_pSceneManager; // シーンマネージャークラスのポインタ
};
