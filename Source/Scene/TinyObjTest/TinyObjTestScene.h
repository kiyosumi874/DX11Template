/**
* @file TinyObjTestScene.h
* @brief ���[�_�[�̃e�X�g�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_12_10
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Scene/Scene.h"

/**
* @class TinyObjTestScene
* @brief TinyObjTestScene�N���X
* @details Scene���p�����Ă���
*/
class TinyObjTestScene : public Scene
{
public:
	/**
	* @fn TinyObjTestScene
	* @brief �R���X�g���N�^
	*/
	TinyObjTestScene();

	/**
	* @fn ~TinyObjTestScene
	* @brief �f�X�g���N�^
	*/
	~TinyObjTestScene() override;

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
	class ObjMesh* m_pObjMesh;
};