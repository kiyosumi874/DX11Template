/**
* @file GameOverScene.h
* @brief ゲームオーバーシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#pragma once
#include "Scene/Scene.h"

/**
* @namespace scene
* @brief シーンの名前空間
*/
namespace scene
{
	/**
	* @class GameOverScene
	* @brief GameOverSceneクラス
	* @details Sceneを継承している
	*/
	class GameOverScene : public Scene
	{
	public:
		/**
		* @fn GameOverScene
		* @brief コンストラクタ
		*/
		GameOverScene();

		/**
		* @fn ~GameOverScene
		* @brief デストラクタ
		*/
		~GameOverScene() override;

		/**
		* @fn Update
		* @brief 更新
		* @return TAG_SCENE シーンの種類
		*/
		TAG_SCENE Update() override;

		/**
		* @fn Draw
		* @brief 描画
		*/
		void Draw() override;

	private:

	};
}