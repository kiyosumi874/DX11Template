/**
* @file TitleScene.h
* @brief �^�C�g���V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Scene/Scene.h"


/**
	* @class TitleScene
	* @brief TitleScene�N���X
	* @details Scene���p�����Ă���
	*/
class TitleScene : public Scene
{
public:
	/**
	* @fn TitleScene
	* @brief �R���X�g���N�^
	*/
	TitleScene();

	/**
	* @fn ~TitleScene
	* @brief �f�X�g���N�^
	*/
	~TitleScene() override;

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
#ifdef _DEBUG
	bool ImageImGuiConfig(Object** pObj, const char* windowName, const char* fileName, const int ID);
#endif // _DEBUG

	void InitCamera();
	void InitStaticMesh();
	void InitHierarchyMesh();
	void InitSkinMesh();
	void InitUI();

};