/**
* @file TitleScene.cpp
* @brief タイトルシーン
* @details Sceneを継承している
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// マクロ定義
#define NOMINMAX // maxとminのエラー回避

// ヘッダーファイルのインクルード
#include "TitleScene.h"
#include <Windows.h>
#include "System/Input/Input.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "imgui.h"
#include "System/Common.h"
#include "Game/Camera/CameraStruct.h"
#include "Game/Component/Primitive/Point/Point.h"
#include "Game/Component/Primitive/Line/Line.h"
#include "Game/Component/Primitive/Triangle/Triangle.h"
#include "Game/Component/Primitive/Quad/Quad.h"
#include "Game/Component/Image/Image.h"
#include "System/File.h"
#include "Game/Component/Mesh/StaticMesh/StaticMesh.h"
#include "Game/Component/Mesh/HierarchyMesh/HierarchyMesh.h"

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
	// camera
	InitCamera();

	// HierarchyMesh
	//InitHierarchyMesh();

	// StaticMesh
	//InitStaticMesh();

	// SkinMesh
	//InitSkinMesh();

	// UI
	InitUI();
	
	// GUI
	setting = new ImguiWrapper::Setting();
	setting->Init(*image, "Test", "test", ImguiWrapper::Mode::All);

	
}

/**
* @fn ~TitleScene
* @brief デストラクタ
*/
TitleScene::~TitleScene()
{
	SAFE_DELETE(image);
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
		return TAG_SCENE::TEST;
	}

#ifdef _DEBUG
	image->Update();
	ImguiWrapper::Action(*setting);
#endif // _DEBUG
	// ループが続く
	return TAG_SCENE::NONE;
}

/**
* @fn Draw
* @brief 描画
*/
void TitleScene::Draw()
{
	image->Draw();
	
	MyOutputDebugString("TitleScene\n");
}

#ifdef _DEBUG
bool TitleScene::ImageImGuiConfig(Image*& pImage, const char* windowName, const char* fileName, const int ID)
{
	// using宣言
	using ImGui::Begin; using ImGui::End; using ImGui::DragFloat;using ImGui::SliderFloat;
	using ImGui::Button; using ImGui::SameLine; using ImGui::Text;

	if (pImage->GetID() == ID)
	{
		Begin(windowName);
		auto pos = pImage->GetPos();
		auto scale = pImage->GetScale();
		auto rotate = pImage->GetRotate();
		DragFloat("posX", &pos.x, 0.5f, 0.0f, WINDOW_WIDTH);
		DragFloat("posY", &pos.y, 0.5f, 0.0f, WINDOW_HEIGHT);
		DragFloat("scaleX", &scale.x, 0.005f, 0.0f, 10.0f);
		DragFloat("scaleY", &scale.y, 0.005f, 0.0f, 10.0f);
		DragFloat("rotateZ", &rotate.z, 0.005f, 0.0f, 360.0f * D3DX_PI / 180.0f);
		pImage->SetPos(pos);
		pImage->SetRotate(rotate);
		pImage->SetScale(scale);

		if (Button("Save"))
		{
			File::FileData data(pos.x, pos.y, scale.x, scale.y, rotate.z);
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

void TitleScene::InitCamera()
{
}

void TitleScene::InitStaticMesh()
{
	/*{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		auto staticMesh = obj->AddComponent<StaticMesh>();
		if (FAILED(staticMesh->Init("model/RobotA_pivot.x")))
		{
			MessageBox(0, "StaticMesh初期化失敗", NULL, MB_OK);
		}
		m_objectList.emplace_back(obj);
	}*/
}
void TitleScene::InitHierarchyMesh()
{
	/*{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->position.x = -0.5;
		transform->position.y = -1.0;
		transform->position.z = 1.0;
		auto staticMesh = obj->AddComponent<HierarchyMesh>();
		if (FAILED(staticMesh->Init("model/RobotA_1motion_2truck.x")))
		{
			MessageBox(0, "HierarchyMesh初期化失敗", NULL, MB_OK);
		}
		m_objectList.emplace_back(obj);
	}
	{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->position.x = -0.8;
		transform->position.y = -1.0;
		transform->position.z = 0.5;
		auto staticMesh = obj->AddComponent<HierarchyMesh>();
		if (FAILED(staticMesh->Init("model/Josh_Motion_Wakeup.x")))
		{
			MessageBox(0, "HierarchyMesh初期化失敗", NULL, MB_OK);
		}
		m_objectList.emplace_back(obj);
	}*/
}
void TitleScene::InitSkinMesh()
{
	/*{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->position.x = 0.5;
		auto skinMesh = obj->AddComponent<SkinMesh>();
		if (FAILED(skinMesh->Init("model/Hand_animation_1motion_2truck.x")))
		{
			MessageBox(0, "SkinMesh初期化失敗", NULL, MB_OK);
		}
		m_objectList.emplace_back(obj);
	}*/
}

void TitleScene::InitUI()
{
	// UI0
	{
		File::FileData data;
		data = File::LoadFile("OutputData/UI0.txt");

		image = new Image();
		image->SetPos(Vector3D(data.posX, data.posY, 0.0f));
		image->SetRotate(Vector3D(0.0f,0.0f,data.rotateZ));
		image->Init(Vector3D(data.posX, data.posY, 0), Vector3D(160, 151, 0), "Resource/0.png", true, 0);
		/*Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->scale.x = data.scaleX;
		transform->scale.y = data.scaleY;
		transform->rotation.z = data.rotateZ;
		auto ui = obj->AddComponent<Image>();
		ui->Init(Vector3D(data.posX, data.posY, 0), Vector3D(160, 151, 0), "Resource/0.png", true, 0);
		m_objectList.emplace_back(obj);*/
	}

	// UI1
	{
		/*File::FileData data;
		data = File::LoadFile("OutputData/UI1.txt");

		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->scale.x = data.scaleX;
		transform->scale.y = data.scaleY;
		transform->rotation.z = data.rotateZ;
		auto ui = obj->AddComponent<Image>();
		ui->Init(Vector3D(data.posX, data.posY, 0), Vector3D(32, 32, 0), "Resource/VisualStudio 2022.png", true, 1);
		m_objectList.emplace_back(obj);*/
	}
}
