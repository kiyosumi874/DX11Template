/**
* @file StaticMesh.h
* @brief StaticMeshの描画
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// ヘッダーファイルのインクルード
#include "StaticMesh.h"
#include "D3D9/Direct3D9.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Component/Object.h"
#include <d3dx11.h>
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3dx11.lib")

/**
* @fn Start
* @brief 生成したときに最初に一回だけ走る関数
*/
void StaticMesh::Start()
{
}

void StaticMesh::Update()
{
}

void StaticMesh::Draw()
{
	UINT mask = 0xffffffff;
	Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, mask);

	// using宣言
	using D3D11::SetVertexBuffer;
	SetVertexBuffer(&m_pVertexBuffer, m_pMesh->GetNumBytesPerVertex());

	//使用するシェーダーのセット
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	
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
	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 sg;
		//ワールド行列を渡す
		sg.world = world;
		D3DXMatrixTranspose(&sg.world, &sg.world);
		//ワールド、カメラ、射影行列を渡す
		sg.worldVewProj = world * data.matrixView * data.matrixProj;
		D3DXMatrixTranspose(&sg.worldVewProj, &sg.worldVewProj);
		//ライトの方向を渡す
		D3DXVECTOR3 light(0, 0.5, -1);
		D3DXVec3Normalize(&light, &light);
		sg.lightDir = (D3DXVECTOR4)light;
		//視点位置を渡す
		sg.eye = D3DXVECTOR4(data.pos.x, data.pos.y, data.pos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer0));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer0, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	//頂点インプットレイアウトをセット
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//属性の数だけ、それぞれの属性のインデックスバッファ－を描画
	for (DWORD i = 0; i < m_numAttribute; i++)
	{
		//使用されていないマテリアル対策
		if (m_materialVec[m_attributeID[i]].numFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		Direct3D11::GetDeviceContext()->IASetIndexBuffer(m_pIndexBufferVec[i], DXGI_FORMAT_R32_UINT, 0);
		//Direct3D11::GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBuffer1 sg;
			sg.ambient = m_materialVec[m_attributeID[i]].ambient;//アンビエントををシェーダーに渡す
			sg.diffuse = m_materialVec[m_attributeID[i]].diffuse;//ディフューズカラーをシェーダーに渡す
			sg.specular = m_materialVec[m_attributeID[i]].specular;//スペキュラーをシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer1));
			Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer1, 0);
		}
		Direct3D11::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		Direct3D11::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		//テクスチャーをシェーダーに渡す
		if (m_materialVec[m_attributeID[i]].pTexture)
		{
			Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &m_pSampler);
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &m_materialVec[m_attributeID[i]].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブをレンダリング
		Direct3D11::GetDeviceContext()->DrawIndexed(m_materialVec[m_attributeID[i]].numFace * 3, 0, 0);
	}

}

void StaticMesh::Terminate()
{
	for (auto it : m_pIndexBufferVec)
	{
		SAFE_RELEASE(it);
	}
	m_pIndexBufferVec.clear();
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexLayout);
	m_materialVec.clear();
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pD3DXMtrlBuffer);
	SAFE_RELEASE(m_pMesh);
}

/**
* @fn Init
* @brief 初期化
*/
HRESULT StaticMesh::Init(const char* fileName)
{
	if (FAILED(LoadXMesh(fileName)))
	{
		return E_FAIL;
	}

	// マテリアルをpD3DXMtrlBufferから取得
	if (FAILED(FetchMaterial()))
	{
		SAFE_RELEASE(m_pD3DXMtrlBuffer);
		MSG("FetchMaterial");
		return E_FAIL;
	}
	SAFE_RELEASE(m_pD3DXMtrlBuffer); // もう使わん

	// インデックスバッファー作成
	if (FAILED(CreateIndexBuffer()))
	{
		MSG("CreateIndexBuffer");
		return E_FAIL;
	}

	// バーテックスバッファー作成
	if (FAILED(CreateVertexBuffer()))
	{
		MSG("CreateVertexBuffer");
		return E_FAIL;
	}

	//テクスチャー用サンプラー作成
	if (FAILED(CreateSampler()))
	{
		MSG("CreateSampler");
		return E_FAIL;
	}

	// using宣言
	using D3D11::CreateVertexShader;
	using D3D11::CreatePixelShader;
	using D3D11::CreateInputLayout;
	using D3D11::CreateConstantBuffer;

	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3D10Blob* pCompiledShader = NULL;

	if (m_useTexture)
	{
		//ブロブからバーテックスシェーダー作成
		CreateVertexShader(&m_pVertexShader, &pCompiledShader, "Shader/Mesh.hlsl", "VS");
		//頂点インプットレイアウトを定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = 3;
		CreateInputLayout(&m_pVertexLayout, &pCompiledShader, layout, numElements);
		//ブロブからピクセルシェーダー作成
		CreatePixelShader(&m_pPixelShader, &pCompiledShader, "Shader/Mesh.hlsl", "PS");
	}
	else
	{
		//ブロブからバーテックスシェーダー作成
		CreateVertexShader(&m_pVertexShader, &pCompiledShader, "Shader/Mesh.hlsl", "VS_NoTex");
		//頂点インプットレイアウトを定義
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = 2;
		CreateInputLayout(&m_pVertexLayout, &pCompiledShader, layout, numElements);
		//ブロブからピクセルシェーダー作成
		CreatePixelShader(&m_pPixelShader, &pCompiledShader, "Shader/Mesh.hlsl", "PS_NoTex");
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタントバッファー作成　変換行列渡し用
	CreateConstantBuffer(&m_pConstantBuffer0, sizeof(ConstantBuffer0));
	CreateConstantBuffer(&m_pConstantBuffer1, sizeof(ConstantBuffer1));

	return S_OK;
}

/**
* @fn LoadXMesh
* @brief xfileをロードする
* @param[in] fileName ファイルの名前
* @return HRESULT S_OKで成功
*/
HRESULT StaticMesh::LoadXMesh(const char* fileName)
{

	if (FAILED(D3DXLoadMeshFromXA(fileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		Direct3D9::GetDevice(), NULL, &m_pD3DXMtrlBuffer, NULL,
		&m_numMaterial, &m_pMesh)))
	{
		MessageBoxA(NULL, "Xファイルの読み込みに失敗しました", NULL, MB_OK);
		return E_FAIL;
	}

	// この時点で、ファイルから取り出したメッシュデータが、Dx9のD3DXメッシュに入っている、
	// あとは、そこから好きな情報を取り出してDx11の自前メッシュに利用するだけ。

	

	return S_OK;
}

HRESULT StaticMesh::FetchMaterial()
{
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)m_pD3DXMtrlBuffer->GetBufferPointer();
	m_materialVec.resize(m_numMaterial);
	for (DWORD i = 0; i < m_numMaterial; i++)
	{
		//アンビエント
		m_materialVec[i].ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_materialVec[i].ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_materialVec[i].ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_materialVec[i].ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//ディフューズ
		m_materialVec[i].diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_materialVec[i].diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_materialVec[i].diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_materialVec[i].diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//スペキュラー
		m_materialVec[i].specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_materialVec[i].specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_materialVec[i].specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_materialVec[i].specular.w = d3dxMaterials[i].MatD3D.Specular.a;
		//テクスチャーがあれば
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_useTexture = true;
			strcpy_s(m_materialVec[i].textureName, d3dxMaterials[i].pTextureFilename);
			//テクスチャーを作成
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Direct3D11::GetDevice(),
				m_materialVec[i].textureName, NULL, NULL, &m_materialVec[i].pTexture, NULL)))
			{
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT StaticMesh::CreateIndexBuffer()
{
	//インデックスバッファーを作成
	m_pIndexBufferVec.resize(m_numMaterial);
	//メッシュの属性情報を得る。属性情報でインデックスバッファーから細かいマテリアルごとのインデックスバッファを分離できる
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	m_pMesh->GetAttributeTable(NULL, &m_numAttribute);
	pAttrTable = new D3DXATTRIBUTERANGE[m_numAttribute];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_numAttribute)))
	{
		MessageBoxA(0, "属性テーブル取得失敗", "", MB_OK);
		return E_FAIL;
	}
	//D3DXMESHの場合、ロックしないとD3DXインデックスバッファーから取り出せないのでロックする。
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//属性ごとのインデックスバッファを作成
	for (DWORD i = 0; i < m_numAttribute; i++)
	{
		m_attributeID[i] = pAttrTable[i].AttribId;
		//Dx9のD3DMESHのインデックスバッファーからの情報で、Dx11のインデックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//大きいインデックスバッファ内のオフセット(*3を忘れずに）

		if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pIndexBufferVec[i])))
		{
			return E_FAIL;
		}
		m_materialVec[m_attributeID[i]].numFace = pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;
	m_pMesh->UnlockIndexBuffer();
	return S_OK;
}

HRESULT StaticMesh::CreateVertexBuffer()
{
	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	//D3DXMESHの場合、ロックしないとD3DXバーテックスバッファーから取り出せないのでロックする。
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	MyVertex* pvVertex = NULL;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pvVertex = (MyVertex*)pVertices;
		//Dx9のD3DMESHのバーテックスバッファーからの情報で、Dx11のバーテックスバッファを作成
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pvVertex;

		//テクスチャー座標がマイナス対策
		if (m_useTexture)
		{
			for (int i = 0; i < m_pMesh->GetNumVertices(); i++)
			{
				if (pvVertex[i].tex.x < 0)
				{
					pvVertex[i].tex.x += 1;
				}
				if (pvVertex[i].tex.y < 0)
				{
					pvVertex[i].tex.y += 1;
				}
			}
		}

		if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
			return E_FAIL;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);
	return S_OK;
}

HRESULT StaticMesh::CreateSampler()
{
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampler)))
	{
		return E_FAIL;
	}

	return S_OK;
}
