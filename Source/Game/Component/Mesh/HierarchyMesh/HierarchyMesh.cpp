/**
* @file HierarchyMesh.h
* @brief HierarchyMeshの描画(まだ完全ではない)
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// ヘッダーファイルのインクルード
#include "HierarchyMesh.h"
#include "D3D9/Direct3D9.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Component/Object.h"
#include <d3dx11.h>
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>
#include "System/Input/Input.h"

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3dx11.lib")

#pragma warning(disable:4996)

HRESULT MY_H_HIERARCHY::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	HRESULT hr = S_OK;
	MYHFRAME* pFrame;
	*ppNewFrame = NULL;

	pFrame = new MYHFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	pFrame->Name = new CHAR[lstrlenA(Name) + 1];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy(pFrame->Name, Name);
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);
	pFrame->pMeshContainer = NULL;
	pFrame->pFrameSibling = NULL;
	pFrame->pFrameFirstChild = NULL;
	*ppNewFrame = pFrame;

	return S_OK;
}

//
//HRESULT MY_HIERARCHY::CreateMeshContainer
//メッシュコンテナーを作成する
HRESULT MY_H_HIERARCHY::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD* pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppMeshContainer)
{
	HRESULT hr;
	MYHMESHCONTAINER* pMeshContainer = NULL;
	int iFacesAmount;
	int iMaterial;
	LPDIRECT3DDEVICE9 pDevice = NULL;
	LPD3DXMESH pMesh = NULL;
	*ppMeshContainer = NULL;

	pMesh = pMeshData->pMesh;
	pMeshContainer = new MYHMESHCONTAINER;
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer, sizeof(MYHMESHCONTAINER));

	pMeshContainer->Name = new CHAR[strlen(Name) + 1];
	if (!pMeshContainer->Name)
	{
		return E_FAIL;
	}
	strcpy(pMeshContainer->Name, Name);
	pMesh->GetDevice(&pDevice);
	iFacesAmount = pMesh->GetNumFaces();

	// 当該メッシュが法線を持たない場合は法線を追加する
	if (!(pMesh->GetFVF() & D3DFVF_NORMAL))
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		hr = pMesh->CloneMeshFVF(pMesh->GetOptions(),
			pMesh->GetFVF() | D3DFVF_NORMAL,
			pDevice, &pMeshContainer->MeshData.pMesh);
		if (FAILED(hr))
		{
			return E_FAIL;
		}

		pMesh = pMeshContainer->MeshData.pMesh;
		D3DXComputeNormals(pMesh, NULL);
	}
	else
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;
		pMesh->AddRef();
	}
	pMeshContainer->NumMaterials = max(1, NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[iFacesAmount * 3];
	if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
	{
		return E_OUTOFMEMORY;
	}

	memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * iFacesAmount * 3);
	memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);

	if (NumMaterials > 0)
	{
		memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * NumMaterials);

		for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
		{
			if (pMaterials[iMaterial].pTextureFilename != NULL)
			{
				pMeshContainer->pMaterials[iMaterial].pTextureFilename = new char[strlen(pMaterials[iMaterial].pTextureFilename) + 1];
				strcpy(pMeshContainer->pMaterials[iMaterial].pTextureFilename, pMaterials[iMaterial].pTextureFilename);
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
	*ppMeshContainer = pMeshContainer;
	(*ppMeshContainer)->pMaterials[0].pTextureFilename;
	pMeshContainer = NULL;

	return S_OK;
}

//
//HRESULT MY_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree) 
//フレームを破棄する
HRESULT MY_H_HIERARCHY::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	if (pFrameToFree->pFrameFirstChild)
	{
		DestroyFrame(pFrameToFree->pFrameFirstChild);
	}
	if (pFrameToFree->pFrameSibling)
	{
		DestroyFrame(pFrameToFree->pFrameSibling);
	}

	SAFE_DELETE_ARRAY(pFrameToFree->Name);
	SAFE_DELETE(pFrameToFree);

	return S_OK;
}
//
//HRESULT MY_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
//メッシュコンテナーを破棄する
HRESULT MY_H_HIERARCHY::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
{
	int iMaterial;
	MYHMESHCONTAINER* pMeshContainer = (MYHMESHCONTAINER*)pMeshContainerBase;

	SAFE_RELEASE(pMeshContainer->pSkinInfo);
	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	if (pMeshContainer->ppTextures != NULL)
	{
		for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
		{
			SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);

		}
		SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
	}
	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
	SAFE_DELETE(pMeshContainer);

	return S_OK;
}

void HierarchyMesh::Start()
{
}

void HierarchyMesh::Update()
{
	m_pAnimController->AdvanceTime(0.005, NULL);
	// 仮
	if (Input::IsDown(ButtonID::BUTTON_ID_A/*キーボードはx*/))
	{
		ChangeAnimSet(0);
	}
	if (Input::IsDown(ButtonID::BUTTON_ID_B/*キーボードはv*/))
	{
		ChangeAnimSet(1);
	}
}

void HierarchyMesh::Draw()
{
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT mask = 0xffffffff;
	Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, mask);

	D3DXMATRIX world;

	// アフィン変換
	{
		auto transform = m_parent->GetComponent<Transform>();

		D3DXMATRIX scale;
		D3DXMATRIX rotate;
		D3DXMATRIX pos;

		D3DXMatrixIdentity(&world); // 行列の初期化

		D3DXMatrixScaling(&scale, transform->scale.x, transform->scale.y, transform->scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotate, transform->rotation.y, transform->rotation.x, transform->rotation.z);
		D3DXMatrixTranslation(&pos, transform->position.x, transform->position.y, transform->position.z);

		// DirectXは行優先なのでScaleから乗算
		D3DXMatrixMultiply(&world, &world, &scale);
		D3DXMatrixMultiply(&world, &world, &rotate);
		D3DXMatrixMultiply(&world, &world, &pos);
	}

	UpdateHierarchyMatrices(m_pFrameRoot, &world);
	DrawFrame(m_pFrameRoot);
}

void HierarchyMesh::Terminate()
{
	SAFE_DELETE(m_pHierarchy);
	SAFE_RELEASE(m_pAnimController);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexLayoutNoTex);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexShaderNoTex);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pPixelShaderNoTex);
	SAFE_RELEASE(m_pConstantBufferMesh);
	SAFE_RELEASE(m_pConstantBufferMaterial);
	SAFE_RELEASE(m_pSampler);
	for (int i = 0; i < 100; i++)
	{
		SAFE_RELEASE(m_pAnimSet[i]);
	}
}

HRESULT HierarchyMesh::Init(const char* fileName)
{
	if (FAILED(LoadXAnimMesh(fileName)))
	{
		return E_FAIL;
	}

	if (FAILED(InitShader()))
	{
		return E_FAIL;
	}
	return S_OK;
}

HRESULT HierarchyMesh::LoadXAnimMesh(const char* fileName)
{
	m_pHierarchy = new MY_H_HIERARCHY;
	if (FAILED(D3DXLoadMeshHierarchyFromXA(fileName, D3DXMESH_MANAGED, Direct3D9::GetDevice(),
		m_pHierarchy, NULL, &m_pFrameRoot, &m_pAnimController)))
	{
		MessageBoxA(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}
	//この時点で、ファイルから取り出した全フレームがm_pFrameRootに入っている、
	//またアニメーションをコントロールするにはm_pAnimControllerが初期化されているはずなので、それを使う。

	//あとは、そこから必要な情報をとりだしつつ、かくフレームごとにアプリ独自のメッシュを構築していく
	BuildAllMesh(m_pFrameRoot);

	//アニメーショントラックを得る 　本サンプル添付のXファイルの場合は2セットだけだが100個までに対応できる
	for (DWORD i = 0; i < m_pAnimController->GetNumAnimationSets(); i++)
	{
		m_pAnimController->GetAnimationSet(i, &m_pAnimSet[i]);
	}

	return S_OK;
}

//D３DXMESHからアプリメッシュを作成する
void HierarchyMesh::BuildAllMesh(D3DXFRAME* pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		CreateAppMeshFromD3DXMesh(pFrame);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		BuildAllMesh(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		BuildAllMesh(pFrame->pFrameFirstChild);
	}
}

HRESULT HierarchyMesh::CreateAppMeshFromD3DXMesh(LPD3DXFRAME p)
{
	//この時点で、ファイルから取り出したメッシュデータが、Dx9のD3DXメッシュに入っている、あとは、そこから好きな情報を取り出してDx11の自前メッシュに利用するだけ。
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	MYHFRAME* pFrame = (MYHFRAME*)p;

	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;//D3DXメッシュ（ここから・・・）
	PartsMesh* pAppMesh = new PartsMesh;//アプリメッシュ（・・・ここにメッシュデータをコピーする）
	pAppMesh->useTexture = false;

	pAppMesh->numMaterial = pFrame->pMeshContainer->NumMaterials;

	pAppMesh->pMaterial = new MyHMaterial[pAppMesh->numMaterial];
	pAppMesh->ppIndexBuffer = new ID3D11Buffer * [pAppMesh->numMaterial];

	for (DWORD i = 0; i < pAppMesh->numMaterial; i++)
	{
		//アンビエント
		pAppMesh->pMaterial[i].ambient.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.r;
		pAppMesh->pMaterial[i].ambient.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.g;
		pAppMesh->pMaterial[i].ambient.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.b;
		pAppMesh->pMaterial[i].ambient.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Ambient.a;
		//ディフューズ
		pAppMesh->pMaterial[i].diffuse.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.r;
		pAppMesh->pMaterial[i].diffuse.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.g;
		pAppMesh->pMaterial[i].diffuse.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.b;
		pAppMesh->pMaterial[i].diffuse.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Diffuse.a;
		//スペキュラー
		pAppMesh->pMaterial[i].specular.x = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.r;
		pAppMesh->pMaterial[i].specular.y = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.g;
		pAppMesh->pMaterial[i].specular.z = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.b;
		pAppMesh->pMaterial[i].specular.w = pFrame->pMeshContainer->pMaterials[i].MatD3D.Specular.a;
		//テクスチャーがあれば
		if (pFrame->pMeshContainer->pMaterials[i].pTextureFilename != NULL &&
			lstrlenA(pFrame->pMeshContainer->pMaterials[i].pTextureFilename) > 0)
		{
			pAppMesh->useTexture = true;
			strcpy(pAppMesh->pMaterial[i].textureName, pFrame->pMeshContainer->pMaterials[i].pTextureFilename);
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Direct3D11::GetDevice(),
				pAppMesh->pMaterial[i].textureName, NULL, NULL, &pAppMesh->pMaterial[i].pTexture, NULL)))
			{
				MessageBoxA(0, "テクスチャ作成失敗", "", MB_OK);
				return E_FAIL;
			}
		}
		//インデックスバッファーを作成

		//それに先立ち、メッシュの属性情報を得る。属性情報でインデックスバッファーから細かいマテリアルごとのインデックスバッファを分離できる
		D3DXATTRIBUTERANGE* pAttrTable = NULL;
		DWORD NumAttr = 0;
		pD3DXMesh->GetAttributeTable(pAttrTable, &NumAttr);
		pAttrTable = new D3DXATTRIBUTERANGE[NumAttr];
		if (FAILED(pD3DXMesh->GetAttributeTable(pAttrTable, &NumAttr)))
		{
			MessageBoxA(0, "属性テーブル取得失敗", "", MB_OK);
			return E_FAIL;
		}

		//D3DXMESHの場合、ロックしないとD3DXインデックスバッファーから取り出せないのでロックする。

		//2バイトのインデックスの場合もありえる・・・
		DWORD* pIndex = (DWORD*)new DWORD[pD3DXMesh->GetNumFaces() * 3];

		LPDIRECT3DINDEXBUFFER9 pIB;
		pD3DXMesh->GetIndexBuffer(&pIB);
		D3DINDEXBUFFER_DESC desc;
		pIB->GetDesc(&desc);

		if (desc.Format == D3DFMT_INDEX16)
		{
			WORD* wordIndex = NULL;
			pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&wordIndex);
			for (int i = 0; i < pD3DXMesh->GetNumFaces() * 3; i++)
			{
				pIndex[i] = wordIndex[i];
			}
		}
		else if (desc.Format == D3DFMT_INDEX32)
		{
			DWORD* dwordIndex = NULL;
			pD3DXMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&dwordIndex);
			memcpy(pIndex, dwordIndex, pD3DXMesh->GetNumFaces() * 3);
		}

		//マテリアルごとのインデックスバッファを作成
		for (DWORD i = 0; i < NumAttr; i++)
		{
			//Dx9のD3DMESHのインデックスバッファーからの情報で、Dx11のインデックスバッファを作成
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(DWORD) * pAttrTable[i].FaceCount * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//大きいインデックスバッファ内のオフセット
			InitData.SysMemPitch = 0;
			InitData.SysMemSlicePitch = 0;
			if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &pAppMesh->ppIndexBuffer[i])))
			{
				MessageBoxA(0, "マテリアルごとのインデックスバッファを作成失敗", "", MB_OK);
				return E_FAIL;
			}
			pAppMesh->pMaterial[i].numFace = pAttrTable[i].FaceCount;
		}


		delete[] pAttrTable;
		SAFE_RELEASE(pIB);
		pD3DXMesh->UnlockIndexBuffer();
		delete pIndex;
	}
	//バーテックスバッファーを作成

	//D3DXMESHの場合、ロックしないとD3DXバーテックスバッファーから取り出せないのでロックする。
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pD3DXMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = pD3DXMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	MyHVertexNoTex* pvVertex = NULL;
	MyHVertex* pvVertexTex = NULL;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		//Dx9のD3DMESHのバーテックスバッファーからの情報で、Dx11のアプリ定義バーテックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		if (!pAppMesh->useTexture)
		{
			pvVertex = (MyHVertexNoTex*)pVertices;
			bd.ByteWidth = sizeof(MyHVertexNoTex) * pD3DXMesh->GetNumVertices();
			InitData.pSysMem = pvVertex;
		}
		else
		{
			pvVertexTex = (MyHVertex*)pVertices;
			bd.ByteWidth = sizeof(MyHVertex) * pD3DXMesh->GetNumVertices();
			InitData.pSysMem = pvVertexTex;
		}
		if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &pAppMesh->pVertexBuffer)))
		{
			MessageBoxA(0, "バーテックスバッファを作成失敗", "", MB_OK);
			return E_FAIL;

		}

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);

	pFrame->pPartsMesh = pAppMesh;

	return S_OK;
}
//
//
//
HRESULT HierarchyMesh::InitShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3D10Blob* pCompiledShader = NULL;
	ID3D10Blob* pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(Direct3D11::GetDevice()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
	{
		return E_FAIL;
	}
	//テクスチャーなしシェーダー
	SAFE_RELEASE(pErrors);
	SAFE_RELEASE(pCompiledShader);
	if (FAILED(D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "VS_NoTex", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	if (FAILED(Direct3D11::GetDevice()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShaderNoTex)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "バーテックスシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	//テクスチャーなし
	D3D11_INPUT_ELEMENT_DESC layout2[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	numElements = sizeof(layout2) / sizeof(layout2[0]);
	//頂点インプットレイアウトを作成
	if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layout2, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayoutNoTex)))
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//テクスチャーなし
	if (FAILED(D3DX11CompileFromFile("Shader/Mesh.hlsl", NULL, NULL, "PS_NoTex", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl読み込み失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShaderNoTex)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "ピクセルシェーダー作成失敗", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferMesh);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferMesh)))
	{
		return E_FAIL;
	}
	//コンスタントバッファー作成  マテリアル渡し用
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(ConstantBufferMaterial);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferMaterial)))
	{
		return E_FAIL;
	}
	return S_OK;
}
//
//
//
void HierarchyMesh::UpdateHierarchyMatrices(D3DXFRAME* p, LPD3DXMATRIX pParentMatrix)
{
	MYHFRAME* pFrame = (MYHFRAME*)p;

	if (pParentMatrix != NULL)
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix * *pParentMatrix;
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}

	if (pFrame->pFrameSibling != NULL)
	{
		UpdateHierarchyMatrices(pFrame->pFrameSibling, pParentMatrix);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		UpdateHierarchyMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
	}
}


//
//
void HierarchyMesh::DrawFrame(LPD3DXFRAME p)
{
	MYHFRAME* pFrame = (MYHFRAME*)p;
	PartsMesh* pPartsMesh = pFrame->pPartsMesh;
	if (pPartsMesh != NULL)
	{
		DrawPartsMesh(pPartsMesh, pFrame->CombinedTransformationMatrix);
	}

	if (pFrame->pFrameSibling != NULL)
	{
		DrawFrame(pFrame->pFrameSibling);
	}

	if (pFrame->pFrameFirstChild != NULL)
	{
		DrawFrame(pFrame->pFrameFirstChild);
	}
}
//
//
//
void HierarchyMesh::DrawPartsMesh(PartsMesh* pPartsMesh, D3DXMATRIX World)
{

	CameraData data;
	ZeroMemory(&data, sizeof(CameraData));
	bool isSuccess = TellCameraData::GetCameraData(&data, CAMERA_NUMBER::CAMERA_0);

	if (!isSuccess)
	{
		// エラーメッセージ
		MyOutputDebugString("カメラ取得失敗");
	}

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBufferMesh, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBufferMesh sg;
		//ワールド行列を渡す
		sg.world = World;
		D3DXMatrixTranspose(&sg.world, &sg.world);
		//ワールド、カメラ、射影行列を渡す
		sg.worldVewProj = World * data.matrixView * data.matrixProj;
		D3DXMatrixTranspose(&sg.worldVewProj, &sg.worldVewProj);
		//ライトの方向を渡す
		//ライトの方向を渡す
		D3DXVECTOR3 light(0, 0.5, -1);
		D3DXVec3Normalize(&light, &light);
		sg.lightDir = (D3DXVECTOR4)light;
		//視点位置を渡す
		sg.eye = D3DXVECTOR4(data.pos.x, data.pos.y, data.pos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBufferMesh));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBufferMesh, 0);
	}
	//このコンスタントバッファーを使うシェーダーの登録
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBufferMesh);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBufferMesh);

	//マテリアルごとにレンダリング
	UINT stride = 0;
	if (pPartsMesh->useTexture)
	{
		stride = sizeof(MyHVertex);
		//頂点インプットレイアウトをセット
		Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);
		//使用するシェーダーの登録
		Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
		Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	}
	else
	{
		stride = sizeof(MyHVertexNoTex);
		//頂点インプットレイアウトをセット
		Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayoutNoTex);
		//使用するシェーダーの登録
		Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShaderNoTex, NULL, 0);
		Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShaderNoTex, NULL, 0);
	}
	//バーテックスバッファーをセット
	UINT offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &pPartsMesh->pVertexBuffer, &stride, &offset);

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (DWORD i = 0; i < pPartsMesh->numMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (pPartsMesh->pMaterial[i].numFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		Direct3D11::GetDeviceContext()->IASetIndexBuffer(pPartsMesh->ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBufferMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBufferMaterial sg;
			sg.ambient = pPartsMesh->pMaterial[i].ambient;//アンビエントをシェーダーに渡す
			sg.diffuse = pPartsMesh->pMaterial[i].diffuse;//ディフューズカラーをシェーダーに渡す
			sg.specular = pPartsMesh->pMaterial[i].specular;//スペキュラーをシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBufferMaterial));
			Direct3D11::GetDeviceContext()->Unmap(m_pConstantBufferMaterial, 0);
		}
		Direct3D11::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBufferMaterial);
		Direct3D11::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_pConstantBufferMaterial);
		//テクスチャーをシェーダーに渡す
		if (pPartsMesh->pMaterial[i].textureName[0] != NULL)
		{
			Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &m_pSampler);
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &pPartsMesh->pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブをレンダリング
		Direct3D11::GetDeviceContext()->DrawIndexed(pPartsMesh->pMaterial[i].numFace * 3, 0, 0);
	}
}


void HierarchyMesh::ChangeAnimSet(int index)
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
