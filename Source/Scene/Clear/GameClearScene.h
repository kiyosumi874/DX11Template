/**
* @file GameClearScene.h
* @brief �Q�[���N���A�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Scene/Scene.h"

/**
* @class GameClearScene
* @brief GameClearScene�N���X
* @details Scene���p�����Ă���
*/
class GameClearScene : public Scene
{
public:
	/**
	* @fn GameClearScene
	* @brief �R���X�g���N�^
	*/
	GameClearScene();

	/**
	* @fn ~GameClearScene
	* @brief �f�X�g���N�^
	*/
	~GameClearScene() override;

	/**
	* @fn Update
	* @brief �X�V
	* @return TAG_SCENE �V�[���̎��
	*/
	TAG_SCENE Update() override;

	/**
	* @fn Draw
	* @brief �`��
	*/
	void Draw() override;

private:
};