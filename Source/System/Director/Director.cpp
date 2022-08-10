/**
* @file Director.h
* @brief �v���O�����̊Ǘ�
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Director.h"
#include "System/Common.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Fps/Fps.h"
#include "System/Input/Input.h"
#include "Game/Camera/TellCameraData.h"

/**
* @fn Director
* @brief �R���X�g���N�^
*/
Director::Director()
{
	// �N���X�������������
	ZeroMemory(this, sizeof(Director));
	Direct3D11::CreateInstance();
	Input::CreateInstance();
	Fps::CreateInstance();
	ShaderDirector::CreateInstance();
	TellCameraData::CreateInstance();
}

/**
* @fn ~Director
* @brief �f�X�g���N�^
*/
Director::~Director()
{
	SAFE_DELETE(m_pSceneManager);
	TellCameraData::DeleteInstance();
	ShaderDirector::DeleteInstance();
	Fps::DeleteInstance();
	Input::DeleteInstance();
	Direct3D11::DeleteInstance();
	SAFE_DELETE(m_pWindow);
}

/**
* @fn Init
* @brief ������
* @param[in] hInstance �C���X�^���X�n���h��(�A�v���P�[�V�������̂�\���Ă���u�l�v)
* @return HRESULT S_OK�Ő���
*/
HRESULT Director::Init(HINSTANCE hInstance)
{
	// window����
	{
		m_pWindow = new Window();
		if (!m_pWindow) { return E_FAIL; }
		MFAIL(m_pWindow->Init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME), "�E�B���h�E�쐬���s");
	}

	// Direct3D11Wrapper����
	{
		MFAIL(Direct3D11::Init(m_pWindow->GetWindowHandle()), "Direct3D���������s");
	}


	// sceneManager����
	{
		// using�錾
		using scene::TAG_SCENE;
		m_pSceneManager = new SceneManager();
		m_pSceneManager->SetNowScene(TAG_SCENE::TITLE);
	}

	
	return S_OK;
}

/**
* @fn Run
* @brief ���s
*/
void Director::Run()
{
	// ���b�Z�[�W���[�v
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT || Input::IsPress(BUTTON_ID_BACK))
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// true��Ԃ��ꂽ��I��
			if (MainLoop()) { break; }
		}
	}
}

bool Director::MainLoop()
{
	bool isTerm = false;
	Input::Update();
	Fps::Update();
	Fps::Draw();
	Direct3D11::Clear(150.0f, 150.0f, 150.0f);
	isTerm = m_pSceneManager->GameLoop();
	Direct3D11::Present();
	return isTerm;
}
