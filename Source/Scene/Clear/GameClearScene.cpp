/**
* @file GameClearScene.h
* @brief �Q�[���N���A�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "GameClearScene.h"
#include <Windows.h>
#include "System/Input/Input.h"

/**
* @fn GameClearScene
* @brief �R���X�g���N�^
*/
GameClearScene::GameClearScene()
{
}

/**
* @fn ~GameClearScene
* @brief �f�X�g���N�^
*/
GameClearScene::~GameClearScene()
{
}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
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
* @brief �`��
*/
void GameClearScene::Draw()
{
	OutputDebugString("GameClearScene\n");
}
