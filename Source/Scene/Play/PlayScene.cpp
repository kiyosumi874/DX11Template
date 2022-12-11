/**
* @file PlayScene.h
* @brief プレイシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "PlayScene.h"
#include <Windows.h>
#include "System/Input/Input.h"

/**
* @fn PlayScene
* @brief コンストラクタ
*/
PlayScene::PlayScene()
{
	// 未実装
}

/**
* @fn ~PlayScene
* @brief デストラクタ
*/
PlayScene::~PlayScene()
{
	// 未実装
}

/**
* @fn Update
* @brief 更新
* @return TAG_SCENE シーンの種類
*/
TAG_SCENE PlayScene::Update()
{

	if (Input::IsDown(BUTTON_ID_START))
	{
		return TAG_SCENE::OVER;
	}

	return TAG_SCENE::NONE;
}

/**
* @fn Draw
* @brief 描画
*/
void PlayScene::Draw()
{
	OutputDebugString("PlayScene\n");

}