/**
* @file SceneManager.h
* @brief �V�[�����Ǘ�����
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Scene/Scene.h"

/**
* @class SceneManager
* @brief �V�[�����Ǘ�����N���X
*/
class SceneManager
{
public:
	/**
	* @fn SceneManager
	* @brief �R���X�g���N�^
	*/
	SceneManager();

	/**
	* @fn ~SceneManager
	* @brief �f�X�g���N�^
	*/
	~SceneManager();

	/**
	* @fn GameLoop
	* @brief �Q�[�����[�v
	* @return true�ŏI��
	*/
	bool GameLoop();

	/**
	* @fn SetNowScene
	* @brief �V�[�����Z�b�g����
	* @param[in] tag �i�݂����V�[���̃^�O
	*/
	void SetNowScene(const TAG_SCENE& tag);
private:
	/**
	* @fn ClearScene
	* @brief ���̃V�[�����f���[�g
	*/
	void ClearScene();

	Scene* m_nowScene; // ���݂̃V�[���̃|�C���^
};