/**
* @file GameOverScene.h
* @brief ゲームオーバーシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "GameOverScene.h"
#include <Windows.h>
#include "System/Input/Input.h"

// using宣言
using scene::GameOverScene; using scene::TAG_SCENE;

/**
* @fn GameOverScene
* @brief コンストラクタ
*/
GameOverScene::GameOverScene()
{
	// 未実装
}

/**
* @fn ~GameOverScene
* @brief デストラクタ
*/
GameOverScene::~GameOverScene()
{
	// 未実装
}

/**
* @fn Update
* @brief 更新
* @return TAG_SCENE シーンの種類
*/
TAG_SCENE GameOverScene::Update()
{
	if (Input::IsDown(BUTTON_ID_START))
	{
		return TAG_SCENE::TITLE;
	}
	
	return TAG_SCENE::NONE;
}

/**
* @fn Draw
* @brief 描画
*/
void GameOverScene::Draw()
{
	OutputDebugString("GameOverScene\n");

}
