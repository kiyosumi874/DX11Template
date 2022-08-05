/**
* @file Director.h
* @brief プログラムの管理
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// ヘッダーファイルのインクルード
#include "Director.h"
#include "System/Common.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Fps/Fps.h"
#include "System/Input/Input.h"
#include "Game/Camera/TellCameraData.h"

/**
* @fn Director
* @brief コンストラクタ
*/
Director::Director()
{
	// クラスが初期化される
	ZeroMemory(this, sizeof(Director));
	Direct3D11::CreateInstance();
	Input::CreateInstance();
	Fps::CreateInstance();
	ShaderDirector::CreateInstance();
	TellCameraData::CreateInstance();
}

/**
* @fn ~Director
* @brief デストラクタ
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
* @brief 初期化
* @param[in] hInstance インスタンスハンドル(アプリケーション自体を表している「値」)
* @return HRESULT S_OKで成功
*/
HRESULT Director::Init(HINSTANCE hInstance)
{
	// window生成
	{
		m_pWindow = new Window();
		if (!m_pWindow) { return E_FAIL; }
		MFAIL(m_pWindow->Init(hInstance, WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME), "ウィンドウ作成失敗");
	}

	// Direct3D11Wrapper生成
	{
		MFAIL(Direct3D11::Init(m_pWindow->GetWindowHandle()), "Direct3D初期化失敗");
	}


	// sceneManager生成
	{
		// using宣言
		using scene::TAG_SCENE;
		m_pSceneManager = new SceneManager();
		m_pSceneManager->SetNowScene(TAG_SCENE::TITLE);
	}

	
	return S_OK;
}

/**
* @fn Run
* @brief 実行
*/
void Director::Run()
{
	// メッセージループ
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
			// trueを返されたら終了
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
	Direct3D11::Clear(0,0,0);
	isTerm = m_pSceneManager->GameLoop();
	Direct3D11::Present();
	return isTerm;
}
