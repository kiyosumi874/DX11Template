/**
* @file TinyObjTestScene.h
* @brief ���[�_�[�̃e�X�g�V�[��
* @details Scene���p�����Ă���
* @author shiihara_kiyosumi
* @date 2022_12_10
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "TinyObjTestScene.h"
#include <Windows.h>
#include <iostream>
#include "System/Input/Input.h"
#include "D3D11/Direct3D11.h"
#include "Game/Component/Mesh/ObjMesh/ObjMesh.h"
#include <System/Common.h>

/**
* @fn TinyObjTestScene
* @brief �R���X�g���N�^
*/
TinyObjTestScene::TinyObjTestScene()
	: m_pObjMesh(nullptr)
{
	m_pObjMesh = new ObjMesh();
	if (FAILED(m_pObjMesh->Init(Direct3D11::GetDevice(), Direct3D11::GetDeviceContext(), "cottage_obj.obj")))
	{
		MessageBox(0, "ObjMesh���������s", NULL, MB_OK);
		return;
	}
}

/**
* @fn ~TinyObjTestScene
* @brief �f�X�g���N�^
*/
TinyObjTestScene::~TinyObjTestScene()
{
	SAFE_DELETE(m_pObjMesh);
}

/**
* @fn Update
* @brief �X�V
* @return TAG_SCENE �V�[���̎��
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
* @brief �`��
*/
void TinyObjTestScene::Draw()
{
	OutputDebugString("TinyObjTestScene\n");
	D3DXMATRIX mView;
	D3DXMATRIX mProj;	// �r���[�g�����X�t�H�[���i���_���W�ϊ��j
	D3DXVECTOR3 vEyePt(0.0f, 0.0f, -0.5f); //�J�����i���_�j�ʒu
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);//�����ʒu
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);//����ʒu
	D3DXMatrixLookAtLH(&mView, &vEyePt, &vLookatPt, &vUpVec);
	// �v���W�F�N�V�����g�����X�t�H�[���i�ˉe�ϊ��j
	D3DXMatrixPerspectiveFovLH(&mProj, D3DX_PI / 4, (FLOAT)WINDOW_WIDTH / (FLOAT)WINDOW_HEIGHT, 0.1f, 110.0f);
	//�����_�����O
	auto vLight = D3DXVECTOR3(1, 1, -1);
	m_pObjMesh->Render(mView, mProj, vLight, vEyePt);
	m_pObjMesh->m_fYaw += 0.001;
}