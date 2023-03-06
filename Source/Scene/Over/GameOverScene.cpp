/**
* @file GameOverScene.h
* @brief ゲームオーバーシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "GameOverScene.h"
#include <Windows.h>
#include "System/Input/Input.h"
#include "Game/SkinMesh/SkinMesh.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"

/**
* @fn GameOverScene
* @brief コンストラクタ
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
* @brief デストラクタ
*/
GameOverScene::~GameOverScene()
{
	TellCameraData::SubCamera(CAMERA_NUMBER::CAMERA_0);
	SAFE_DELETE(skinMesh);

}

/**
* @fn Update
* @brief 更新
* @return TAG_SCENE シーンの種類
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
* @brief 描画
*/
void GameOverScene::Draw()
{
	OutputDebugString("GameOverScene\n");
	auto tran = Transform();
	tran.SetPos(Vector3D(0.5f, 0.0f, 0.0f));
	tran.SetScale(Vector3D(2.0f, 2.0f, 2.0f));
	skinMesh->Draw(tran);
}
