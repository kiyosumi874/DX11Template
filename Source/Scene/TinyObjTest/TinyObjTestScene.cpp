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
#include "Game/Component/Mesh/ObjMesh/ObjMesh.h"
#include <System/Common.h>

/**
* @fn TinyObjTestScene
* @brief コンストラクタ
*/
TinyObjTestScene::TinyObjTestScene()
	: m_pObjMesh(nullptr)
{
	m_pObjMesh = new ObjMesh();
	if (FAILED(m_pObjMesh->Init(Direct3D11::GetDevice(), Direct3D11::GetDeviceContext(), "cottage_obj.obj")))
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
	SAFE_DELETE(m_pObjMesh);
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
	D3DXMATRIX mView;
	D3DXMATRIX mProj;	// ビュートランスフォーム（視点座標変換）
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -0.5f); //カメラ（視点）位置
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//注視位置
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//上方位置
	D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	// プロジェクショントランスフォーム（射影変換）
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	//レンダリング
	auto vLight = D3DXVECTOR3(1, 1, -1);
	m_pObjMesh->Render(mView, mProj, vLight, vEyePt);
	m_pObjMesh->m_fYaw += 0.001;
}