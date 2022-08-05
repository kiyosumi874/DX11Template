/**
* @file TitleScene.cpp
* @brief タイトルシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "TitleScene.h"
#include <Windows.h>
#include "System/Input/Input.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Primitive/Point/Point.h"
#include "System/Math/Math.h"
#include "System/Common.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"

// using宣言
using math::Vector3D; using scene::TitleScene; using scene::TAG_SCENE;

/**
* @fn TitleScene
* @brief コンストラクタ
*/
TitleScene::TitleScene()
{
	ShaderDirector::CreateShader(SHADER_KIND::PRIMITIVE);
	ShaderDirector::CreateShader(SHADER_KIND::PRIMITIVE2D);

	// camera
	{
		m_pCamera = new Camera();
		m_pCamera->SetCameraNumber(CAMERA_NUMBER::CAMERA_0);
		m_pCamera->SetCameraPosition(0, 0, -2);
		TellCameraData::AddCamera(m_pCamera->GetCameraData());
	}


	auto pos = Vector3D(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, 0.0f);
	m_pPoint = new Point(pos, true);

}

/**
* @fn ~TitleScene
* @brief デストラクタ
*/
TitleScene::~TitleScene()
{
	SAFE_DELETE(m_pPoint);
	TellCameraData::SubCamera(m_pCamera->GetCameraData());
	SAFE_DELETE(m_pCamera);
}

/**
* @fn Update
* @brief 更新
* @return TAG_SCENE シーンの種類
*/
TAG_SCENE TitleScene::Update()
{
	if (Input::IsDown(BUTTON_ID_START))
	{
		// PlaySceneに切り替わる
		return TAG_SCENE::PLAY;
	}

	// ループが続く
	return TAG_SCENE::NONE;
}

/**
* @fn Draw
* @brief 描画
*/
void TitleScene::Draw()
{
	m_pPoint->Draw();

	MyOutputDebugString("TitleScene\n");
}