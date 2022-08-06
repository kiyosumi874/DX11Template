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


	// 2D�œ_��`��
	{
		Object* obj = nullptr;
		for (int i = 0; i < 100; i++)
		{
			for (int j = 0; j < 100; j++)
			{
				obj = new Object;
				auto transform = obj->AddComponent<Transform>();
				transform->m_position.x = 640 / 2 + i * 4 - 50 * 4;
				transform->m_position.y = 480 / 2 + j * 4 - 50 * 4;
				transform->m_position.z = 0.0f;
				auto point = obj->AddComponent<Point>();
				point->Is2D(true);
				m_objectList.emplace_back(obj);
			}
		}
	}

	// 3D�œ_��`��
	{
		/*Object* obj = new Object;
		auto transform = obj->AddComponent<Transform>();
		transform->m_position.x = 0.0f;
		transform->m_position.y = 0.0f;
		transform->m_position.z = 0.0f;
		obj->AddComponent<Point>();
		obj->GetComponent<Point>()->Is2D(false);
		m_objectList.emplace_back(obj);*/
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