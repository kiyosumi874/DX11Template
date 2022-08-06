/**
* @file TitleScene.h
* @brief タイトルシーン
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
	* @class TitleScene
	* @brief TitleSceneクラス
	* @details Sceneを継承している
	*/
	class TitleScene : public Scene
	{
	public:
		/**
		* @fn TitleScene
		* @brief コンストラクタ
		*/
		TitleScene();

		/**
		* @fn ~TitleScene
		* @brief デストラクタ
		*/
		~TitleScene() override;

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
		//Camera* m_pCamera; // カメラクラスのポインタ
	};

}