/**
* @file GameClearScene.h
* @brief ゲームクリアシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "GameClearScene.h"
#include <Windows.h>
#include "System/Input/Input.h"

/**
* @fn GameClearScene
* @brief コンストラクタ
*/
GameClearScene::GameClearScene()
{
}

/**
* @fn ~GameClearScene
* @brief デストラクタ
*/
GameClearScene::~GameClearScene()
{
}

/**
* @fn Update
* @brief 更新
* @return TAG_SCENE シーンの種類
*/
TAG_SCENE GameClearScene::Update()
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
void GameClearScene::Draw()
{
	OutputDebugString("GameClearScene\n");
}
