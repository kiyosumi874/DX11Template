/**
* @file Director.h
* @brief �v���O�����̊Ǘ�
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <Windows.h>
#include "System/Window/Window.h"
#include "Scene/SceneManager/SceneManager.h"

/**
* @class Director
* @brief �v���O�����̊Ǘ��N���X
*/
class Director
{
public:
	/**
	* @fn Director
	* @brief �R���X�g���N�^
	*/
	Director();

	/**
	* @fn ~Director
	* @brief �f�X�g���N�^
	*/
	~Director();

	/**
	* @fn Init
	* @brief ������
	* @param[in] hInstance �C���X�^���X�n���h��(�A�v���P�[�V�������̂�\���Ă���u�l�v)
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT Init(HINSTANCE hInstance);

	/**
	* @fn Run
	* @brief ���s
	*/
	void Run();

private:
	// true�ŏI��,false�ŉ����Ȃ�
	bool MainLoop();

	Window* m_pWindow; // �E�B���h�E�N���X�̃|�C���^
	SceneManager* m_pSceneManager; // �V�[���}�l�[�W���[�N���X�̃|�C���^
};
