/**
* @file SceneManager.h
* @brief シーンを管理する
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "SceneManager.h"
#include "Scene/Title/TitleScene.h"
#include "Scene/Play/PlayScene.h"
#include "Scene/Clear/GameClearScene.h"
#include "Scene/Over/GameOverScene.h"
#include "System/Fps/Fps.h"
#include "System/Input/Input.h"

// using宣言
using scene::SceneManager; using scene::TAG_SCENE;

/**
* @class SceneManager
* @brief シーンを管理するクラス
*/
SceneManager::SceneManager()
	: m_nowScene(nullptr)
{
	// 未実装
}

/**
* @fn ~SceneManager
* @brief デストラクタ
*/
SceneManager::~SceneManager()
{
	ClearScene();
}

/**
* @fn GameLoop
* @brief ゲームループ
* @return trueで終了
*/
bool SceneManager::GameLoop()
{
#ifdef _DEBUG
	if (Input::IsDown(BUTTON_ID_BACK))
	{
		return true;
	}
#endif // _DEBUG

	auto tag = m_nowScene->Update();

	m_nowScene->Draw();

	if (tag == TAG_SCENE::NONE)
	{
		return false;
	}

	ClearScene();
	if (tag == TAG_SCENE::END)
	{
		return true;
	}
	SetNowScene(tag);
	return false;
}

/**
* @fn SetNowScene
* @brief シーンをセットする
* @param[in] tag 進みたいシーンのタグ
*/
void SceneManager::SetNowScene(const TAG_SCENE& tag)
{
	switch (tag)
	{
	case TAG_SCENE::TITLE:
		m_nowScene = new TitleScene();
		break;
	case TAG_SCENE::PLAY:
		m_nowScene = new PlayScene();
		break;
	case TAG_SCENE::CLEAR:
		m_nowScene = new GameClearScene();
		break;
	case TAG_SCENE::OVER:
		m_nowScene = new GameOverScene();
		break;
	case TAG_SCENE::NONE:
		break;
	default:
		break;
	}
}

/**
* @fn ClearScene
* @brief 今のシーンをデリート
*/
void SceneManager::ClearScene()
{
	if (m_nowScene != nullptr)
	{
		delete m_nowScene;
		m_nowScene = nullptr;
	}
	
}
