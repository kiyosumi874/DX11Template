/**
* @file HierarchyMesh.h
* @brief HierarchyMesh�̕`��(�܂����S�ł͂Ȃ�)
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <d3dx9.h>
#include <d3d11.h>
#include "Game/Component/Component.h"
#include "System/Transform.h"
#include "System/Common.h"
#include <vector>

// using�錾
using std::vector;

struct ConstantBufferMesh
{
	D3DXMATRIX world;//���[���h�s��
	D3DXMATRIX worldVewProj;//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 lightDir;//���C�g����
	D3DXVECTOR4 eye;//�J�����ʒu
};

struct ConstantBufferMaterial
{
	D3DXVECTOR4 ambient;//�A���r�G���g��
	D3DXVECTOR4 diffuse;//�f�B�t���[�Y�F
	D3DXVECTOR4 specular;//���ʔ���
};

//�I���W�i���@�}�e���A���\����
struct MyHMaterial
{
	CHAR name[100];
	D3DXVECTOR4 ambient;//�A���r�G���g
	D3DXVECTOR4 diffuse;//�f�B�t���[�Y
	D3DXVECTOR4 specular;//�X�y�L�����[
	CHAR textureName[256];//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	DWORD numFace;//���̃}�e���A���ł���|���S����
	MyHMaterial()
	{
		ZeroMemory(this, sizeof(MyHMaterial));
	}
	~MyHMaterial()
	{
		SAFE_RELEASE(pTexture);
	}
};

//���_�̍\����
struct MyHVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;
};

struct MyHVertexNoTex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
};

//�p�[�c���b�V���@�A�v����`���b�V��(�X�^�e�B�b�N���b�V���̃T���v���ɂ�����CD3DXMESH�N���X�Ɠ����j
struct PartsMesh
{
	DWORD numMaterial;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer** ppIndexBuffer;
	MyHMaterial* pMaterial;

	bool useTexture;
};
//�h���t���[���\����
struct MYHFRAME : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
	PartsMesh* pPartsMesh;
	MYHFRAME()
	{
		ZeroMemory(this, sizeof(MYHFRAME));
	}
};
//���b�V���R���e�i�[�\���� �t���[�����Ŏg�p
struct MYHMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures;
};
//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����N���X�B
class MY_H_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_H_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME*);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD*, LPD3DXSKININFO, LPD3DXMESHCONTAINER*);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

/**
* @class HierarchyMesh
* @brief �����Ȃ����b�V���N���X
*/
class HierarchyMesh : public Component
{
public:
	/**
	* @fn Start
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	void Start() override;

	/**
	* @fn Update
	* @brief �X�V
	*/
	void Update() override;

	/**
	* @fn Draw
	* @brief �`��
	*/
	void Draw() override;

	/**
	* @fn Terminate
	* @brief �I������
	*/
	void Terminate() override;

	/**
	* @fn Init
	* @brief ������
	*/
	HRESULT Init(const char* fileName);

	void ChangeAnimSet(int index);
private:
	/**
	* @fn LoadXMesh
	* @brief xfile�����[�h����
	* @param[in] fileName �t�@�C���̖��O
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT LoadXAnimMesh(const char* fileName);
	HRESULT InitShader();

	void BuildAllMesh(D3DXFRAME* pFrame);
	HRESULT CreateAppMeshFromD3DXMesh(LPD3DXFRAME pFrame);
	void DrawFrame(LPD3DXFRAME p);
	void DrawPartsMesh(PartsMesh* p, D3DXMATRIX World);
	void UpdateHierarchyMatrices(D3DXFRAME* p, LPD3DXMATRIX pParentMatrix);

	//�A�j���֘A
	MY_H_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;


	// resource
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11InputLayout* m_pVertexLayoutNoTex;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11VertexShader* m_pVertexShaderNoTex;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11PixelShader* m_pPixelShaderNoTex;
	ID3D11Buffer* m_pConstantBufferMesh;
	ID3D11Buffer* m_pConstantBufferMaterial;
	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	LPD3DXANIMATIONSET m_pAnimSet[100];//100�܂ł̃A�j���[�V�����Z�b�g�ɑΉ�

};
