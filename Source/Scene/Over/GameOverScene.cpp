/**
* @file GameOverScene.h
* @brief �Q�[���I�[�o�[�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "GameOverScene.h"
#include <Windows.h>
#include "System/Input/Input.h"

// using�錾
using scene::GameOverScene; using scene::TAG_SCENE;

/**
* @fn GameOverScene
* @brief �R���X�g���N�^
*/
GameOverScene::GameOverScene()
{
	// ������
}

/**
* @fn ~GameOverScene
* @brief �f�X�g���N�^
*/
GameOverScene::~GameOverScene()
{
	// ������
}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
*/
TAG_SCENE GameOverScene::Update()
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
void GameOverScene::Draw()
{
	OutputDebugString("GameOverScene\n");

}
