/**
* @file SceneManager.h
* @brief シーンを管理する
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#pragma once
#include "Scene/Scene.h"

/**
* @class SceneManager
* @brief シーンを管理するクラス
*/
class SceneManager
{
public:
	/**
	* @fn SceneManager
	* @brief コンストラクタ
	*/
	SceneManager();

	/**
	* @fn ~SceneManager
	* @brief デストラクタ
	*/
	~SceneManager();

	/**
	* @fn GameLoop
	* @brief ゲームループ
	* @return trueで終了
	*/
	bool GameLoop();

	/**
	* @fn SetNowScene
	* @brief シーンをセットする
	* @param[in] tag 進みたいシーンのタグ
	*/
	void SetNowScene(const TAG_SCENE& tag);
private:
	/**
	* @fn ClearScene
	* @brief 今のシーンをデリート
	*/
	void ClearScene();

	Scene* m_nowScene; // 現在のシーンのポインタ
};