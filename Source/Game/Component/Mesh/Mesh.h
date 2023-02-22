/**
* @file Mesh.h
* @brief Mesh�Ŏg���N���X��\���̂̒�`
* @author shiihara_kiyosumi
* @date 2022_08_18
*/

#pragma once
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include "System/Common.h"

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#define MAX_BONES 255

//�V�F�[�_�[�ɓn���l
struct SHADER_SKIN_GLOBAL0
{
	D3DXVECTOR4 vLightDir;//���C�g����
	D3DXVECTOR4 vEye;//�J�����ʒu
};

//�V�F�[�_�[�ɓn���l
struct SHADER_SKIN_GLOBAL1
{
	D3DXMATRIX mW;//���[���h�s��
	D3DXMATRIX mWVP;//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 vAmbient;//�A���r�G���g��
	D3DXVECTOR4 vDiffuse;//�f�B�t���[�Y�F
	D3DXVECTOR4 vSpecular;//���ʔ���
};

//�V�F�[�_�[�ɓn���l
struct SHADER_GLOBAL_BONES
{
	D3DXMATRIX mBone[MAX_BONES];
	SHADER_GLOBAL_BONES()
	{
		for (int i = 0; i < MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&mBone[i]);
		}
	}
};

//�I���W�i���@�}�e���A���\����
struct MY_SKINMATERIAL
{
	CHAR szName[110];
	D3DXVECTOR4 Ka;//�A���r�G���g
	D3DXVECTOR4 Kd;//�f�B�t���[�Y
	D3DXVECTOR4 Ks;//�X�y�L�����[
	CHAR szTextureName[512];//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;//���̃}�e���A���ł���|���S����
	MY_SKINMATERIAL()
	{
		ZeroMemory(this, sizeof(MY_SKINMATERIAL));
	}
	~MY_SKINMATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

//���_�\����
struct MY_SKINVERTEX
{
	D3DXVECTOR3 vPos;//���_�ʒu
	D3DXVECTOR3 vNorm;//���_�@��
	D3DXVECTOR2 vTex;//UV���W
	UINT bBoneIndex[4];//�{�[���@�ԍ�
	float bBoneWeight[4];//�{�[���@�d��
	MY_SKINVERTEX()
	{
		ZeroMemory(this, sizeof(MY_SKINVERTEX));
	}
};
//�{�[���\����
struct BONE
{
	D3DXMATRIX mBindPose;//�����|�[�Y�i�����ƕς��Ȃ��j
	D3DXMATRIX mNewPose;//���݂̃|�[�Y�i���̓s�x�ς��j
	DWORD dwNumChild;//�q�̐�
	int iChildIndex[50];//�����̎q�́h�C���f�b�N�X" 50�܂�
	CHAR Name[256];

	BONE()
	{
		ZeroMemory(this, sizeof(BONE));
		D3DXMatrixIdentity(&mBindPose);
		D3DXMatrixIdentity(&mNewPose);
	}
};


//�h���t���[���\���� (���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����j
struct MYFRAME : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};
//�h�����b�V���R���e�i�[�\����(
//�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����j
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures;
	DWORD dwWeight;//�d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j
	DWORD dwBoneNum;//�{�[���̐�
	LPD3DXBUFFER pBoneBuffer;//�{�[���E�e�[�u��
	D3DXMATRIX** ppBoneMatrix;//�S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^�[
	D3DXMATRIX* pBoneOffsetMatrices;//�t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^�[	
};
//X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������B
//ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���B
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME*);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
		CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD*, LPD3DXSKININFO, LPD3DXMESHCONTAINER*);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};


class D3DXPARSER
{
public:

	//MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;
	MYMESHCONTAINER* m_pContainer;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	LPD3DXANIMATIONSET m_pAnimSet[100];//100�܂ł̃A�j���[�V�����Z�b�g�ɑΉ�
	HRESULT LoadMeshFromX(LPDIRECT3DDEVICE9, LPCSTR);
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME);
	VOID UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);
	
	int GetNumVertices();
	int GetNumFaces();
	int GetNumMaterials();
	int GetNumUVs();
	int GetNumBones();
	int GetNumBoneVertices(int iBoneIndex);
	DWORD GetBoneVerticesIndices(int iBoneIndex, int iIndexInGroup);
	double GetBoneVerticesWeights(int iBoneIndex, int iIndexInGroup);
	D3DXVECTOR3 GetVertexCoord(DWORD iIndex);
	D3DXVECTOR3 GetNormal(DWORD iIndex);
	D3DXVECTOR2 GetUV(DWORD iIndex);
	int GetIndex(DWORD iIndex);
	D3DXVECTOR4 GetAmbient(int iIndex);
	D3DXVECTOR4 GetDiffuse(int iIndex);
	D3DXVECTOR4 GetSpecular(int iIndex);
	CHAR* GetTexturePath(int index);
	float GetSpecularPower(int iIndex);
	int GeFaceMaterialIndex(int iFaceIndex);
	int GetFaceVertexIndex(int iFaceIndex, int iIndexInFace);
	D3DXMATRIX GetBindPose(int iBoneIndex);
	D3DXMATRIX GetNewPose(int iBoneIndex);
	CHAR* GetBoneName(int iBoneIndex);
	void ChangeAnimSet(int index);
};