/**
* @file SceneManager.h
* @brief �V�[�����Ǘ�����
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "SceneManager.h"
#include "Scene/Title/TitleScene.h"
#include "Scene/Play/PlayScene.h"
#include "Scene/Clear/GameClearScene.h"
#include "Scene/Over/GameOverScene.h"
#include "Scene/TinyObjTest/TinyObjTestScene.h"
#include "System/Fps/Fps.h"
#include "System/Input/Input.h"


/**
* @class SceneManager
* @brief �V�[�����Ǘ�����N���X
*/
SceneManager::SceneManager()
	: m_nowScene(nullptr)
{
	// ������
}

/**
* @fn ~SceneManager
* @brief �f�X�g���N�^
*/
SceneManager::~SceneManager()
{
	ClearScene();
}

/**
* @fn GameLoop
* @brief �Q�[�����[�v
* @return true�ŏI��
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
* @brief �V�[�����Z�b�g����
* @param[in] tag �i�݂����V�[���̃^�O
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
	case TAG_SCENE::TEST:
		m_nowScene = new TinyObjTestScene();
		break;
	case TAG_SCENE::NONE:
		break;
	default:
		break;
	}
}

/**
* @fn ClearScene
* @brief ���̃V�[�����f���[�g
*/
void SceneManager::ClearScene()
{
	if (m_nowScene != nullptr)
	{
		delete m_nowScene;
		m_nowScene = nullptr;
	}
	
}
