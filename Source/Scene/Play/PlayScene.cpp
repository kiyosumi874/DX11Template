/**
* @file PlayScene.h
* @brief �v���C�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "PlayScene.h"
#include <Windows.h>
#include "System/Input/Input.h"

/**
* @fn PlayScene
* @brief �R���X�g���N�^
*/
PlayScene::PlayScene()
{
	// ������
}

/**
* @fn ~PlayScene
* @brief �f�X�g���N�^
*/
PlayScene::~PlayScene()
{
	// ������
}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
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
* @brief �`��
*/
void PlayScene::Draw()
{
	OutputDebugString("PlayScene\n");

}