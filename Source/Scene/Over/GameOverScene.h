/**
* @file GameOverScene.h
* @brief �Q�[���I�[�o�[�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Scene/Scene.h"

class Camera;
class SkinMesh;

/**
* @class GameOverScene
* @brief GameOverScene�N���X
* @details Scene���p�����Ă���
*/
class GameOverScene : public Scene
{
public:
	/**
	* @fn GameOverScene
	* @brief �R���X�g���N�^
	*/
	GameOverScene();

	/**
	* @fn ~GameOverScene
	* @brief �f�X�g���N�^
	*/
	~GameOverScene() override;

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
	Camera* camera;
	SkinMesh* skinMesh;
};