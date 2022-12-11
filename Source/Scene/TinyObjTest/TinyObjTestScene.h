/**
* @file TinyObjTestScene.h
* @brief ローダーのテストシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_12_10
*/

// ヘッダーファイルのインクルード
#pragma once
#include "Scene/Scene.h"

/**
* @class TinyObjTestScene
* @brief TinyObjTestSceneクラス
* @details Sceneを継承している
*/
class TinyObjTestScene : public Scene
{
public:
	/**
	* @fn TinyObjTestScene
	* @brief コンストラクタ
	*/
	TinyObjTestScene();

	/**
	* @fn ~TinyObjTestScene
	* @brief デストラクタ
	*/
	~TinyObjTestScene() override;

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
	class ObjMesh* m_pObjMesh;
};