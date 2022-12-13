/**
* @file TinyObjTestScene.h
* @brief ���[�_�[�̃e�X�g�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_12_10
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "TinyObjTestScene.h"
#include <Windows.h>
#include <iostream>
#include "System/Input/Input.h"
#include "D3D11/Direct3D11.h"
#include "Game/Component/Mesh/ObjMesh/ObjMesh.h"
#include <System/Common.h>
#include "System/Transform.h"

/**
* @fn TinyObjTestScene
* @brief �R���X�g���N�^
*/
TinyObjTestScene::TinyObjTestScene()
	: m_pObjMesh(nullptr)
{
	m_pObjMesh = new ObjMesh();
	if (FAILED(m_pObjMesh->Init(Direct3D11::GetDevice(), Direct3D11::GetDeviceContext(), "default.obj")))
	{
		MessageBox(0, "ObjMesh���������s", NULL, MB_OK);
		return;
	}
}

/**
* @fn ~TinyObjTestScene
* @brief �f�X�g���N�^
*/
TinyObjTestScene::~TinyObjTestScene()
{
	SAFE_DELETE(m_pObjMesh);
}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
*/
TAG_SCENE TinyObjTestScene::Update()
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
void TinyObjTestScene::Draw()
{
	OutputDebugString("TinyObjTestScene\n");
	m_pObjMesh->Draw(Transform());
}