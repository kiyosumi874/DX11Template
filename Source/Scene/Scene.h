/**
* @file Scene.h
* @brief シーンの親
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// 多重インクルード防止
#pragma once

/**
* @enum TAG_SCENE
* @brief シーンの種類
*/
enum class TAG_SCENE
{
	TITLE,
	PLAY,
	CLEAR,
	OVER,
	END,
	TEST,

	NONE
};

/**
* @class Scene
* @brief シーンの親クラス
*/
class Scene
{
public:
	/**
	* @fn Scene
	* @brief コンストラクタ
	*/
	Scene() {}

	/**
	* @fn ~Scene
	* @brief デストラクタ(仮想関数)
	*/
	virtual ~Scene()
	{
	}

	/**
	* @fn Update
	* @brief 更新(純粋仮想関数)
	* @return TAG_SCENE シーンの種類
	*/
	virtual TAG_SCENE Update() = 0;

	/**
	* @fn Draw
	* @brief 描画(純粋仮想関数)
	*/
	virtual void Draw() = 0;
};