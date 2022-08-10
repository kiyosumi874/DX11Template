/**
* @file TitleScene.cpp
* @brief �^�C�g���V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "TitleScene.h"
#include <Windows.h>
#include "System/Input/Input.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Math/Math.h"
#include "System/Common.h"
#include "Game/Camera/Camera.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Camera/CameraStruct.h"
#include "Game/Component/Primitive/Point/Point.h"
#include "Game/Component/Primitive/Line/Line.h"
#include "Game/Component/Primitive/Triangle/Triangle.h"
#include "Game/Component/Primitive/Quad/Quad.h"
#include "Game/Component/Transform/Transform.h"

// using�錾
using scene::TitleScene; using scene::TAG_SCENE;

/**
* @fn TitleScene
* @brief �R���X�g���N�^
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
		camera->SetCameraPosition(0, 0, -2);
		TellCameraData::AddCamera(camera->GetCameraData());
		m_objectList.emplace_back(obj);
	}

	// 2D�Ő���`��
	{
		Object* obj = new Object;
		obj->AddComponent<Transform>();
		auto line = obj->AddComponent<Line>();
		line->Is2D(true);
		line->SetColor(Vector3D(1, 0, 0));
		line->Init(Vector3D(200, 30, 0), Vector3D(500, 300, 0));
		m_objectList.emplace_back(obj);
	}
	
	

	

	// 2D�Ŏl�p�`��`��
	{
		Object* obj = new Object;
		auto tranceform = obj->AddComponent<Transform>();
		tranceform->m_scale = Vector3D(0.3f, 0.3f, 0.3f);
		tranceform->m_position = Vector3D(100, 100, 0.0f);
		auto line = obj->AddComponent<Quad>();
		line->Is2D(true);
		line->SetColor(Vector3D(0, 1.0f, 0));
		line->Init(Vector3D(0, 0, 0), Vector3D(WINDOW_WIDTH, 0, 0), Vector3D(0, WINDOW_HEIGHT, 0), Vector3D(WINDOW_WIDTH, WINDOW_HEIGHT, 0));
		m_objectList.emplace_back(obj);
	}

	// 3D�Ŏl�p�`��`��
	{
		Object* obj = new Object;
		auto tranceform = obj->AddComponent<Transform>();
		tranceform->m_scale = Vector3D(0.3f, 0.3f, 0.3f);
		tranceform->m_rotation = Vector3D(0.0f, 0.0f, 45 * D3DX_PI / 180);
		tranceform->m_position = Vector3D(0.3f, 0.0f, 0.0f);
		auto line = obj->AddComponent<Quad>();
		line->SetColor(Vector3D(0, 0, 0.3f));
		line->Init(Vector3D(-0.5, -0.5, 0), Vector3D(-0.5, 0.5, 0), Vector3D(0.5, -0.5, 0), Vector3D(0.5, 0.5, 0));
		m_objectList.emplace_back(obj);
	}

	// 3D�ŎO�p�`��`��
	{
		Object* obj = new Object;
		auto tranceform = obj->AddComponent<Transform>();
		tranceform->m_rotation = Vector3D(45 * D3DX_PI / 180, 45 * D3DX_PI / 180, 45 * D3DX_PI / 180);
		auto line = obj->AddComponent<Triangle>();
		line->Init(Vector3D(0.0f, 0.5f, 0.0f), Vector3D(0.5f, -0.5f, 0.0f), Vector3D(-0.5f, -0.5f, 0.0f));
		line->SetColor(Vector3D(0, 0.3f, 0.3f));
		m_objectList.emplace_back(obj);
	}

}

/**
* @fn ~TitleScene
* @brief �f�X�g���N�^
*/
TitleScene::~TitleScene()
{
	TellCameraData::SubCamera(CAMERA_NUMBER::CAMERA_0);
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
		return TAG_SCENE::PLAY;
	}

	// �I�u�W�F�N�g�̍X�V
	for (auto obj : m_objectList)
	{
		obj->Update();
	}

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
	for (auto obj : m_objectList)
	{
		obj->Draw();
	}

	MyOutputDebugString("TitleScene\n");
}