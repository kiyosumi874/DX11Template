/**
* @file GameOverScene.h
* @brief �Q�[���I�[�o�[�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "GameOverScene.h"
#include <Windows.h>
#include "System/Input/Input.h"
#include "Game/SkinMesh/SkinMesh.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"

/**
* @fn GameOverScene
* @brief �R���X�g���N�^
*/
GameOverScene::GameOverScene()
{
	// Camera
	camera = new Camera();
	camera->SetCameraNumber(CAMERA_NUMBER::CAMERA_0);
	camera->SetCameraPositionGaze(0, 1.0f, -2.0f, 0, 0, 0);
	TellCameraData::AddCamera(camera->GetCameraData());

	// SkinMesh
	skinMesh = new SkinMesh();
	skinMesh->Init("model/Hand_animation_1motion_2truck.x");
}

/**
* @fn ~GameOverScene
* @brief �f�X�g���N�^
*/
GameOverScene::~GameOverScene()
{
	TellCameraData::SubCamera(CAMERA_NUMBER::CAMERA_0);
	SAFE_DELETE(skinMesh);

}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
*/
TAG_SCENE GameOverScene::Update()
{
	if (Input::IsDown(BUTTON_ID_START))
	{
		return TAG_SCENE::TITLE;
	}
	
	return TAG_SCENE::NONE;
}

/**
* @fn Draw
* @brief �`��
*/
void GameOverScene::Draw()
{
	OutputDebugString("GameOverScene\n");
	auto tran = Transform();
	tran.SetPos(Vector3D(0.5f, 0.0f, 0.0f));
	tran.SetScale(Vector3D(2.0f, 2.0f, 2.0f));
	skinMesh->Draw(tran);
}
