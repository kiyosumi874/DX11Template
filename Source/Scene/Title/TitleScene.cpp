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
#include "System/Math/Math.h"
#include "imgui.h"
#include "System/Common.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"
#include "Game/Component/Primitive/Point/Point.h"
#include "Game/Component/Primitive/Line/Line.h"
#include "Game/Component/Primitive/Triangle/Triangle.h"
#include "Game/Component/Primitive/Quad/Quad.h"
#include "Game/Component/Transform/Transform.h"
#include "Game/Component/Image/Image.h"
#include "System/File.h"
#include "Game/Component/Mesh/StaticMesh/StaticMesh.h"
#include "Game/Component/Mesh/HierarchyMesh/HierarchyMesh.h"
#include "Game/Component/Mesh/SkinMesh/SkinMesh.h"
// using宣言
using scene::TitleScene; using scene::TAG_SCENE;

#ifdef _DEBUG
// ImGuiのための静的変数
const int g_elementNum = 2;
int g_saveButtonCounter[g_elementNum] = {0};
#endif // _DEBUG


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
		Object* obj = new Object;
		auto camera = obj->AddComponent<Camera>();
		camera->SetCameraNumber(CAMERA_NUMBER::CAMERA_0);
		camera->SetCameraPositionGaze(0, 1.0f, -2.0f,0,0,0);
		TellCameraData::AddCamera(camera->GetCameraData());
		m_objectList.emplace_back(obj);
	}

	// HierarchyMesh
	{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->position.x = -0.5;
		transform->position.y = -1.0;
		transform->position.z = 1.0;
		auto staticMesh = obj->AddComponent<HierarchyMesh>();
		staticMesh->Init("model/RobotA_1motion_2truck.x");
		m_objectList.emplace_back(obj);
	}
	{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->position.x = -0.8;
		transform->position.y = -1.0;
		transform->position.z = 0.5;
		auto staticMesh = obj->AddComponent<HierarchyMesh>();
		staticMesh->Init("model/Josh_Motion_Wakeup.x");
		m_objectList.emplace_back(obj);
	}

	// staticMesh
	{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		auto staticMesh = obj->AddComponent<StaticMesh>();
		staticMesh->Init("model/RobotA_pivot.x");
		m_objectList.emplace_back(obj);
	}

	// SkinMesh
	{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->position.x = 0.5;
		auto skinMesh = obj->AddComponent<SkinMesh>();
		skinMesh->Init("model/Hand_animation_1motion_2truck.x");
		m_objectList.emplace_back(obj);
	}

	// UI0
	{
		File::FileData data;
		data = File::LoadFile("OutputData/UI0.txt");

		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->scale.x = data.scaleX;
		transform->scale.y = data.scaleY;
		transform->rotation.z = data.rotateZ;
		auto ui = obj->AddComponent<Image>();
		ui->Init(Vector3D(data.posX, data.posY, 0), Vector3D(160, 151, 0), "Resource/0.png", true, 0);
		m_objectList.emplace_back(obj);
	}

	// UI1
	{
		File::FileData data;
		data = File::LoadFile("OutputData/UI1.txt");

		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->scale.x = data.scaleX;
		transform->scale.y = data.scaleY;
		transform->rotation.z = data.rotateZ;
		auto ui = obj->AddComponent<Image>();
		ui->Init(Vector3D(data.posX, data.posY, 0), Vector3D(32, 32, 0), "Resource/VisualStudio 2022.png", true, 1);
		m_objectList.emplace_back(obj);
	}
	
	

	
	
}

/**
* @fn ~TitleScene
* @brief デストラクタ
*/
TitleScene::~TitleScene()
{
	TellCameraData::SubCamera(CAMERA_NUMBER::CAMERA_0);
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

#ifdef _DEBUG
	// ImGuiの"SaveComplete!!"の表示する時間の管理
	for (int i = 0; i < g_elementNum; i++)
	{
		if (g_saveButtonCounter[i] > 0)
		{
			if (++g_saveButtonCounter[i] > 90)
			{
				g_saveButtonCounter[i] = 0;
			}
		}
	}
#endif // _DEBUG

	// オブジェクトの更新
	for (auto obj : m_objectList)
	{
#ifdef _DEBUG
		if (obj->GetComponent<Image>() != nullptr)
		{
			ImageImGuiConfig(&obj, "UI0", "OutputData/UI0.txt", 0);
			ImageImGuiConfig(&obj, "UI1", "OutputData/UI1.txt", 1);
		}
#endif // _DEBUG
		obj->Update();
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
	// オブジェクトの描画
	for (auto& obj : m_objectList)
	{
		obj->Draw();
	}

	MyOutputDebugString("TitleScene\n");
}

#ifdef _DEBUG
bool scene::TitleScene::ImageImGuiConfig(Object** pObj, const char* windowName, const char* fileName, const int ID)
{
	// using宣言
	using ImGui::Begin; using ImGui::End; using ImGui::DragFloat;
	using ImGui::SetNextWindowSize; using ImGui::SliderFloat;
	using ImGui::Button; using ImGui::SameLine; using ImGui::Text;

	auto obj = *pObj;

	if (obj->GetComponent<Image>()->GetID() == ID)
	{
		Begin(windowName);
		auto& posX = obj->GetComponent<Transform>()->position.x;
		auto& posY = obj->GetComponent<Transform>()->position.y;
		auto& scaleX = obj->GetComponent<Transform>()->scale.x;
		auto& scaleY = obj->GetComponent<Transform>()->scale.y;
		auto& rotateZ = obj->GetComponent<Transform>()->rotation.z;
		DragFloat("posX", &posX, 0.5f, 0.0f, WINDOW_WIDTH);
		DragFloat("posY", &posY, 0.5f, 0.0f, WINDOW_HEIGHT);
		DragFloat("scaleX", &scaleX, 0.005f, 0.0f, 10.0f);
		DragFloat("scaleY", &scaleY, 0.005f, 0.0f, 10.0f);
		DragFloat("rotateZ", &rotateZ, 0.005f, 0.0f, 360.0f * D3DX_PI / 180.0f);
		if (Button("Save"))
		{
			File::FileData data(posX, posY, scaleX, scaleY, rotateZ);
			File::OutputFile(fileName, data);
			g_saveButtonCounter[ID]++;
		}
		if (g_saveButtonCounter[ID] != 0)
		{
			SameLine(); Text("SaveComplete!!");
		}

		End();
		return true;
	}
	return false;
}
#endif // _DEBUG
