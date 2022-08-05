/**
* @file PlayScene.h
* @brief �v���C�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Scene/Scene.h"

/**
* @namespace scene
* @brief �V�[���̖��O���
*/
namespace scene
{
	/**
	* @class PlayScene
	* @brief PlayScene�N���X
	* @details Scene���p�����Ă���
	*/
	class PlayScene : public Scene
	{
	public:
		/**
		* @fn PlayScene
		* @brief �R���X�g���N�^
		*/
		PlayScene();

		/**
		* @fn ~PlayScene
		* @brief �f�X�g���N�^
		*/
		~PlayScene() override;

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
}