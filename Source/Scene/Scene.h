/**
* @file Scene.h
* @brief �V�[���̐e
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ���d�C���N���[�h�h�~
#pragma once

/**
* @enum TAG_SCENE
* @brief �V�[���̎��
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
* @brief �V�[���̐e�N���X
*/
class Scene
{
public:
	/**
	* @fn Scene
	* @brief �R���X�g���N�^
	*/
	Scene() {}

	/**
	* @fn ~Scene
	* @brief �f�X�g���N�^(���z�֐�)
	*/
	virtual ~Scene()
	{
	}

	/**
	* @fn Update
	* @brief �X�V(�������z�֐�)
	* @return TAG_SCENE �V�[���̎��
	*/
	virtual TAG_SCENE Update() = 0;

	/**
	* @fn Draw
	* @brief �`��(�������z�֐�)
	*/
	virtual void Draw() = 0;
};