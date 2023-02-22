/**
* @file TitleScene.cpp
* @brief �^�C�g���V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �}�N����`
#define NOMINMAX // max��min�̃G���[���

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "TitleScene.h"
#include <Windows.h>
#include "System/Input/Input.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "imgui.h"
#include "System/Common.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"
#include "Game/Component/Primitive/Point/Point.h"
#include "Game/Component/Primitive/Line/Line.h"
#include "Game/Component/Primitive/Triangle/Triangle.h"
#include "Game/Component/Primitive/Quad/Quad.h"
#include "Game/Component/Image/Image.h"
#include "System/File.h"
#include "Game/Component/Mesh/StaticMesh/StaticMesh.h"
#include "Game/Component/Mesh/HierarchyMesh/HierarchyMesh.h"
#include "Game/Component/Mesh/SkinMesh/SkinMesh.h"

#ifdef _DEBUG
// ImGui�̂��߂̐ÓI�ϐ�
const int g_elementNum = 2;
int g_saveButtonCounter[g_elementNum] = {0};
#endif // _DEBUG


/**
* @fn TitleScene
* @brief �R���X�g���N�^
*/
TitleScene::TitleScene()
{
	ShaderDirector::CreateShader(SHADER_KIND::PRIMITIVE);
	ShaderDirector::CreateShader(SHADER_KIND::PRIMITIVE2D);

	// camera
	//InitCamera();

	// HierarchyMesh
	//InitHierarchyMesh();

	// StaticMesh
	//InitStaticMesh();

	// SkinMesh
	//InitSkinMesh();

	// UI
	InitUI();
	
	/*setting = new ImguiWrapper::Setting<Image>();
	setting->Init(*m_image, "Test", "test", ImguiWrapper::Mode::All);*/
}

/**
* @fn ~TitleScene
* @brief �f�X�g���N�^
*/
TitleScene::~TitleScene()
{
	TellCameraData::SubCamera(CAMERA_NUMBER::CAMERA_0);
	//SAFE_DELETE(m_image);
}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
*/
TAG_SCENE TitleScene::Update()
{
	if (Input::IsDown(BUTTON_ID_START))
	{
		// PlayScene�ɐ؂�ւ��
		return TAG_SCENE::TEST;
	}

#ifdef _DEBUG
	// ImGui��"SaveComplete!!"�̕\�����鎞�Ԃ̊Ǘ�
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

	m_image->Update();
	// �I�u�W�F�N�g�̍X�V
//	for (auto obj : m_objectList)
//	{
//#ifdef _DEBUG
//		if (obj->GetComponent<Image>() != nullptr)
//		{
//			ImageImGuiConfig(&obj, "UI0", "OutputData/UI0.txt", 0);
//			ImageImGuiConfig(&obj, "UI1", "OutputData/UI1.txt", 1);
//		}
//#endif // _DEBUG
//		obj->Update();
//	}

#ifdef _DEBUG
	ImageImGuiConfig(m_image, "UI0", "OutputData/UI0.txt", 0);
	//ImguiWrapper::Action(*setting);
#endif // _DEBUG


	// ���[�v������
	return TAG_SCENE::NONE;
}

/**
* @fn Draw
* @brief �`��
*/
void TitleScene::Draw()
{
	// �I�u�W�F�N�g�̕`��
	/*for (auto& obj : m_objectList)
	{
		obj->Draw();
	}*/
	m_image->Draw();
	MyOutputDebugString("TitleScene\n");
}

#ifdef _DEBUG
bool TitleScene::ImageImGuiConfig(Image*& pImage, const char* windowName, const char* fileName, const int ID)
{
	// using�錾
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
	{
		m_camera = new Camera();
		m_camera->SetCameraNumber(CAMERA_NUMBER::CAMERA_0);
		m_camera->SetCameraPositionGaze(0, 1.0f, -2.0f, 0, 0, 0);
		TellCameraData::AddCamera(m_camera->GetCameraData());
		/*Object* obj = new Object;
		auto camera = obj->AddComponent<Camera>();
		camera->SetCameraNumber(CAMERA_NUMBER::CAMERA_0);
		camera->SetCameraPositionGaze(0, 1.0f, -2.0f, 0, 0, 0);
		TellCameraData::AddCamera(camera->GetCameraData());
		m_objectList.emplace_back(obj);*/
	}
}

void TitleScene::InitStaticMesh()
{
	/*{
		Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		auto staticMesh = obj->AddComponent<StaticMesh>();
		if (FAILED(staticMesh->Init("model/RobotA_pivot.x")))
		{
			MessageBox(0, "StaticMesh���������s", NULL, MB_OK);
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
			MessageBox(0, "HierarchyMesh���������s", NULL, MB_OK);
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
			MessageBox(0, "HierarchyMesh���������s", NULL, MB_OK);
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
			MessageBox(0, "SkinMesh���������s", NULL, MB_OK);
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

		m_image = new Image();
		m_image->SetPos(Vector3D(data.posX, data.posY, 0.0f));
		m_image->SetRotate(Vector3D(0.0f,0.0f,data.rotateZ));
		m_image->Init(Vector3D(data.posX, data.posY, 0), Vector3D(160, 151, 0), "Resource/0.png", true, 0);
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
