#include "Mesh.h"

#pragma warning(disable:4996)


//�t���[�����쐬����
HRESULT MY_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	HRESULT hr = S_OK;
	MYFRAME* pFrame;

	*ppNewFrame = NULL;

	pFrame = new MYFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	pFrame->Name = new CHAR[strlen(Name) + 1];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy(pFrame->Name, Name);

	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	*ppNewFrame = pFrame;

	return S_OK;
}

//
//HRESULT MY_HIERARCHY::CreateMeshContainer
//���b�V���R���e�i�[���쐬����
HRESULT MY_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppMeshContainer)
{
	*ppMeshContainer = NULL;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER* pMeshContainer = new MYMESHCONTAINER;
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	pMeshContainer->Name = new CHAR[strlen(Name) + 1];
	strcpy(pMeshContainer->Name, Name);

	memcpy(&pMeshContainer->MeshData, pMeshData, sizeof(pMeshContainer->MeshData));
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	int NumPoly = pMeshContainer->MeshData.pMesh->GetNumFaces();
	//���b�V���𕡐�����B�@�Ȃ����X�L�����b�V�����ƁA���̊֐��𔲂��������D3DX�����Ń��b�V���|�C���^�[�����������Ȃ��Ă��܂��̂ŁB
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMeshContainer->MeshData.pMesh->GetDevice(&pDevice);
	LPD3DXMESH pMesh = NULL;
	pMeshContainer->MeshData.pMesh->CloneMesh(NULL, NULL, pDevice, &pMesh);
	//SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	pMeshContainer->MeshData.pMesh = pMesh;

	//���b�V���̃}�e���A���ݒ�
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[pMeshContainer->MeshData.pMesh->GetNumFaces() * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_FAIL;
	}
	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * NumPoly * 3);

	if (NumMaterials > 0)
	{
		for (int i = 0; i < NumMaterials; i++)
		{
			memcpy(&pMeshContainer->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));
			if (pMaterials[i].pTextureFilename != NULL)
			{
				pMeshContainer->pMaterials[i].pTextureFilename = new CHAR[512];
				strcpy(pMeshContainer->pMaterials[i].pTextureFilename, pMaterials[i].pTextureFilename);
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	//���Y���b�V�����X�L�����������Ă���ꍇ�i�X�L�����b�V���ŗL�̏����j
	if (pSkinInfo != NULL)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for (DWORD i = 0; i < dwBoneNum; i++)
		{
			memcpy(&pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i), sizeof(D3DMATRIX));
		}
	}
	//���[�J���ɐ����������b�V���R���e�i�[���Ăяo�����ɃR�s�[����
	*ppMeshContainer = pMeshContainer;

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//�t���[����j������
HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	SAFE_DELETE_ARRAY(pFrameToFree->Name);

	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	SAFE_DELETE(pFrameToFree);

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//���b�V���R���e�i�[��j������
HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	if (pMeshContainer->pBoneBuffer != NULL)
	{
		SAFE_RELEASE(pMeshContainer->pBoneBuffer);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);
	}

	SAFE_DELETE(pMeshContainer);

	return S_OK;
}

//
//
//
HRESULT D3DXPARSER::AllocateBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	MYFRAME* pFrame = NULL;
	DWORD dwBoneNum = 0;

	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	if (pMeshContainer->pSkinInfo == NULL)
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX * [dwBoneNum];

	for (DWORD i = 0; i < dwBoneNum; i++)
	{
		pFrame = (MYFRAME*)D3DXFrameFind(m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName(i));
		if (pFrame == NULL)
		{
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}
//
//
//
HRESULT D3DXPARSER::AllocateAllBoneMatrices(LPD3DXFRAME pFrame)
{
	if (pFrame->pMeshContainer != NULL)
	{
		if (FAILED(AllocateBoneMatrix(pFrame->pMeshContainer)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameSibling != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameSibling)))
		{
			return E_FAIL;
		}
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		if (FAILED(AllocateAllBoneMatrices(pFrame->pFrameFirstChild)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}
//
//
//
HRESULT D3DXPARSER::LoadMeshFromX(LPDIRECT3DDEVICE9 pDevice9, LPCSTR FileName)
{
	// X�t�@�C������A�j���[�V�������b�V����ǂݍ��ݍ쐬����
	m_pHierarchy = new MY_HIERARCHY;
	if (FAILED(
		D3DXLoadMeshHierarchyFromXA(FileName, D3DXMESH_MANAGED, pDevice9, m_pHierarchy,
			NULL, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", FileName, MB_OK);
		return E_FAIL;
	}
	//�{�[�����������肠��
	AllocateAllBoneMatrices(m_pFrameRoot);
	//���b�V���R���e�i�[�𓾂�
	if (m_pFrameRoot->pMeshContainer)
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pMeshContainer;
	}
	else if (m_pFrameRoot->pFrameFirstChild->pMeshContainer)
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pFrameFirstChild->pMeshContainer;
	}
	else//����������ȏ�[���m�[�h��X�t�@�C���̏ꍇ�́A�K�X���̕�����ǉ����邱��
	{
		m_pContainer = (MYMESHCONTAINER*)m_pFrameRoot->pFrameFirstChild->pFrameFirstChild->pMeshContainer;
	}
	//�A�j���[�V�����g���b�N�𓾂� �@�{�T���v���Y�t��X�t�@�C���̏ꍇ��2�Z�b�g��������100�܂łɑΉ��ł���
	for (DWORD i = 0; i < m_pAnimController->GetNumAnimationSets(); i++)
	{
		m_pAnimController->GetAnimationSet(i, &m_pAnimSet[i]);
	}

	return S_OK;
}
//
//VOID UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
//�t���[�����̃��b�V�����Ƀ��[���h�ϊ��s����X�V����
VOID D3DXPARSER::UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix)
{
	MYFRAME* pFrame = (MYFRAME*)pFrameBase;

	if (pParentMatrix != NULL)
	{
		D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if (pFrame->pFrameSibling != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
	}
	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}
//
//
//
int D3DXPARSER::GetNumVertices()
{
	return m_pContainer->MeshData.pMesh->GetNumVertices();
}
//
//
//
int D3DXPARSER::GetNumFaces()
{
	return m_pContainer->MeshData.pMesh->GetNumFaces();
}
//
//
//
int D3DXPARSER::GetNumMaterials()
{
	return m_pContainer->NumMaterials;
}
//
//
//
int D3DXPARSER::GetNumUVs()
{
	return m_pContainer->MeshData.pMesh->GetNumVertices();
}
//
//
//�w�肳�ꂽ�{�[�����e�����y�ڂ����_����Ԃ�
int D3DXPARSER::GetNumBoneVertices(int iBoneIndex)
{
	return m_pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
}
//
//
//�w�肳�ꂽ�{�[�����e�����y�ڂ����_�̃C���f�b�N�X��Ԃ� ��2�����́A�e�����󂯂钸�_�̃C���f�b�N�X�O���[�v���̃C���f�b�N�X�i�C���f�b�N�X���Ⴂ���j
//�Ⴆ�΃{�[���ɉe���������钸�_���S���Ƃ��āA���̃{�[���ɉe����������S�̒��_�̂���2�Ԗڂ̃C���f�b�N�X��m�肽���ꍇ�́AiIndexInGroup��1���w�肷��i0�X�^�[�g�Ȃ̂Łj
DWORD D3DXPARSER::GetBoneVerticesIndices(int iBoneIndex, int iIndexInGroup)
{
	int Num = m_pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
	DWORD* pVerts = new DWORD[Num];
	float* pWights = new float[Num];

	if (FAILED(m_pContainer->pSkinInfo->GetBoneInfluence(iBoneIndex, pVerts, pWights)))
	{
		return E_FAIL;
	}
	return pVerts[iIndexInGroup];
}
//
//
//�w�肳�ꂽ�{�[�����e�����y�ڂ����_�̃{�[���E�F�C�g��Ԃ� ��2�����́A�e�����󂯂钸�_�̃C���f�b�N�X�O���[�v���̃C���f�b�N�X�i�C���f�b�N�X���Ⴂ���j
//�Ⴆ�΃{�[���ɉe���������钸�_���S���Ƃ��āA���̃{�[���ɉe����������S�̒��_�̂���2�Ԗڂ̒��_�̃{�[���E�F�C�g��m�肽���ꍇ�́AiIndexInGroup��1���w�肷��i0�X�^�[�g�Ȃ̂Łj
double D3DXPARSER::GetBoneVerticesWeights(int iBoneIndex, int iIndexInGroup)
{
	int Num = m_pContainer->pSkinInfo->GetNumBoneInfluences(iBoneIndex);
	DWORD* pVerts = new DWORD[Num];
	float* pWights = new float[Num];

	if (FAILED(m_pContainer->pSkinInfo->GetBoneInfluence(iBoneIndex, pVerts, pWights)))
	{
		return E_FAIL;
	}
	return pWights[iIndexInGroup];
}
//
//
//���W
D3DXVECTOR3 D3DXPARSER::GetVertexCoord(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	D3DXVECTOR3* pCoord = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index * Stride;
		pCoord = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pCoord;
}
//
//
//�@��
D3DXVECTOR3 D3DXPARSER::GetNormal(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	D3DXVECTOR3* pNormal = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index * Stride;
		pVertices += sizeof(D3DXVECTOR3);//���W���i�߂� 
		pNormal = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pNormal;
}
//
//
//�e�N�X�`���[���W
D3DXVECTOR2 D3DXPARSER::GetUV(DWORD Index)
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pContainer->MeshData.pMesh->GetVertexBuffer(&pVB);
	DWORD Stride = m_pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	D3DXVECTOR2* pUV = NULL;

	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pVertices += Index * Stride;
		pVertices += sizeof(D3DXVECTOR3);//���W���i�߂�
		pVertices += sizeof(D3DXVECTOR3);//�@�����i�߂�
		pUV = (D3DXVECTOR2*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	return *pUV;
}
//
//
//�C���f�b�N�X�o�b�t�@���̎w�肵���ʒu�i�C���f�b�N�X�C���f�b�N�X�j�ɂ��钸�_�C���f�b�N�X��Ԃ�
int D3DXPARSER::GetIndex(DWORD Index)
{
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	m_pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[Index];

	m_pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}
//
//
//
D3DXVECTOR4 D3DXPARSER::GetAmbient(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Ambient;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}
//
//
//
D3DXVECTOR4 D3DXPARSER::GetDiffuse(int iIndex)
{
	D3DXCOLOR color;
	color = m_pContainer->pMaterials[iIndex].MatD3D.Diffuse;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}
//
//
//
D3DXVECTOR4 D3DXPARSER::GetSpecular(int iIndex)
{
	D3DXCOLOR color;

	color = m_pContainer->pMaterials[iIndex].MatD3D.Specular;
	return D3DXVECTOR4(color.a, color.r, color.g, color.b);
}
//
//
//
CHAR* D3DXPARSER::GetTexturePath(int index)
{
	CHAR* p = NULL;
	p = m_pContainer->pMaterials[index].pTextureFilename;
	return p;
}
//
//
//
float D3DXPARSER::GetSpecularPower(int iIndex)
{
	float power;

	power = m_pContainer->pMaterials[iIndex].MatD3D.Power;
	return power;
}
//
//
//���̃|���S�����A�ǂ̃}�e���A���ł��邩��Ԃ� 
int D3DXPARSER::GeFaceMaterialIndex(int iFaceIndex)
{
	int MaterialIndex = 0;
	DWORD* pBuf = NULL;
	if (SUCCEEDED(m_pContainer->MeshData.pMesh->LockAttributeBuffer(D3DLOCK_READONLY, &pBuf)))
	{
		MaterialIndex = pBuf[iFaceIndex];
	}
	else
	{
		MessageBoxA(0, "�����o�b�t�@�̃��b�N���s", "", MB_OK);
	}
	return MaterialIndex;
}
//
//
//iFaceIndex�Ԗڂ̃|���S�����`������3���_�̒��ŁAiIndexInFace�Ԗ�[0,2]�̒��_�̃C���f�b�N�X��Ԃ�
int D3DXPARSER::GetFaceVertexIndex(int iFaceIndex, int iIndexInFace)
{
	//�C���f�b�N�X�o�b�t�@�[�𒲂ׂ�Ε�����
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	m_pContainer->MeshData.pMesh->LockIndexBuffer(D3DLOCK_READONLY, (VOID**)&pIndex);

	VertIndex = pIndex[iFaceIndex * 3 + iIndexInFace];

	m_pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}
//
//
//
D3DXMATRIX D3DXPARSER::GetBindPose(int iBoneIndex)
{
	return *m_pContainer->pSkinInfo->GetBoneOffsetMatrix(iBoneIndex);
}
//
//
//���̊֐�����Ԃ��Ƃ�Update����Ɩ��ʂȂ̂ŁARender�֐����s����Update������̂Ƃ��Ă��̂܂ܓn���B
//�e�N���X�����̃N���X��UpdateMatrices�����s���Ȃ��ꍇ�́AUpdateMatrices���Ȃ��ƍs�񂪍ŐV�ɂȂ�Ȃ��̂ŗ��ӁB
D3DXMATRIX D3DXPARSER::GetNewPose(int iBoneIndex)
{
	MYMESHCONTAINER* pMyContaiber = (MYMESHCONTAINER*)m_pContainer;
	return *pMyContaiber->ppBoneMatrix[iBoneIndex];
}

//
//
//
CHAR* D3DXPARSER::GetBoneName(int iBoneIndex)
{
	return (CHAR*)m_pContainer->pSkinInfo->GetBoneName(iBoneIndex);
}
//
//
//
int D3DXPARSER::GetNumBones()
{
	return m_pContainer->pSkinInfo->GetNumBones();
}
//
//
//
void D3DXPARSER::ChangeAnimSet(int index)
{
	D3DXTRACK_DESC TrackDesc;
	ZeroMemory(&TrackDesc, sizeof(TrackDesc));

	TrackDesc.Weight = 1;
	TrackDesc.Speed = 1;
	TrackDesc.Enable = 1;
	m_pAnimController->SetTrackDesc(0, &TrackDesc);
	m_pAnimController->SetTrackAnimationSet(0, m_pAnimSet[index]);
	m_pAnimController->SetTrackEnable(index, true);
}