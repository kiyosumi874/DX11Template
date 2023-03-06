/**
* @file TinyObjTestScene.h
* @brief ローダーのテストシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_12_10
*/

// ヘッダーファイルのインクルード
#include "TinyObjTestScene.h"
#include <Windows.h>
#include <iostream>
#include "System/Input/Input.h"
#include "D3D11/Direct3D11.h"
#include "Game/ObjMesh/ObjMesh.h"
#include <System/Common.h>
#include "System/Transform.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"

/**
* @fn TinyObjTestScene
* @brief コンストラクタ
*/
TinyObjTestScene::TinyObjTestScene()
	: objMesh(nullptr)
{
	camera = new Camera();
	camera->SetCameraNumber(CAMERA_NUMBER::CAMERA_0);
	camera->SetCameraPositionGaze(0, 1.0f, -2.0f, 0, 0, 0);
	TellCameraData::AddCamera(camera->GetCameraData());

	objMesh = new ObjMesh();
	if (FAILED(objMesh->Init(Direct3D11::GetDevice(), Direct3D11::GetDeviceContext(), "model/CatGirl.obj")))
	{
		MessageBox(0, "ObjMesh初期化失敗", NULL, MB_OK);
		return;
	}
}

/**
* @fn ~TinyObjTestScene
* @brief デストラクタ
*/
TinyObjTestScene::~TinyObjTestScene()
{
	TellCameraData::SubCamera(CAMERA_NUMBER::CAMERA_0);
	SAFE_DELETE(objMesh);
}

/**
* @fn Update
* @brief 更新
* @return TAG_SCENE シーンの種類
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
* @brief 描画
*/
void TinyObjTestScene::Draw()
{
	OutputDebugString("TinyObjTestScene\n");
	auto tran = Transform();
	tran.SetPos(Vector3D(0.0f, -0.5f, 0.0f));
	static float rot = 0.0f;
	rot += 0.01f;
	tran.SetRotate(Vector3D(0.0f, rot,0.0f));
	objMesh->Draw(tran);
}