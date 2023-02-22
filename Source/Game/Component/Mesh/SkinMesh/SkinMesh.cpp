/**
* @file SkinMesh.cpp
* @brief SkinMeshの描画(まだ完全ではない)
* @author shiihara_kiyosumi
* @date 2022_08_18
*/

// ヘッダーファイルのインクルード
#include "SkinMesh.h"
#include "D3D11/Direct3D11.h"
#include "D3D9/Direct3D9.h"
#include "Game/Component/Object.h"
#include "System/Transform.h"
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>

#pragma warning(disable:4996)

/**
* @fn SkinMesh
* @breif コンストラクタ
*/
SkinMesh::SkinMesh()
{
}

/**
* @fn ~SkinMesh
* @breif デストラクタ
*/
SkinMesh::~SkinMesh()
{
	delete[] m_BoneArray;
	delete[] m_pMaterial;
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pConstantBufferLight);
	SAFE_RELEASE(m_pConstantBufferWVPMaterial);
	SAFE_RELEASE(m_pConstantBufferBone);
	/*for (int i = 0; i < m_dwNumMaterial; i++)
	{
		SAFE_RELEASE(m_ppIndexBuffer[i]);
	}*/
	delete[] m_ppIndexBuffer;
	SAFE_DELETE(m_pMeshParser);
}

/**
* @fn Draw
* @breif 描画
* @param[in] transform 描画位置
*/
void SkinMesh::Draw(const Transform& transform)
{
	const auto& devCon11 = Direct3D11::GetDeviceContext();

	//使用するシェーダーのセット
	devCon11->VSSetShader(m_pVertexShader, NULL, 0);
	devCon11->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX world;

	// アフィン変換
	{
		D3DXMATRIX scale;
		D3DXMATRIX rotate;
		D3DXMATRIX pos;

		D3DXMatrixIdentity(&world); // 行列の初期化

		D3DXMatrixScaling(&scale, transform.GetScale().x, transform.GetScale().y, transform.GetScale().z);
		D3DXMatrixRotationYawPitchRoll(&rotate, transform.GetRotate().y, transform.GetRotate().x, transform.GetRotate().z);
		D3DXMatrixTranslation(&pos, transform.GetPos().x, transform.GetPos().y, transform.GetPos().z);

		// DirectXは行優先なのでScaleから乗算
		D3DXMatrixMultiply(&world, &world, &scale);
		D3DXMatrixMultiply(&world, &world, &rotate);
		D3DXMatrixMultiply(&world, &world, &pos);
	}



	// ボーンの情報をシェーダーに渡す
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devCon11->Map(m_pConstantBufferBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_GLOBAL_BONES sg;
			for (int i = 0; i < m_iNumBone; i++)
			{
				D3DXMATRIX mat = GetCurrentPoseMatrix(i);
				D3DXMatrixTranspose(&mat, &mat);
				sg.mBone[i] = mat;
			}
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_GLOBAL_BONES));
			devCon11->Unmap(m_pConstantBufferBone, 0);
		}
		devCon11->VSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
		devCon11->PSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
	}


	//バーテックスバッファーをセット（バーテックスバッファーは一つでいい）
	UINT stride = sizeof(MY_SKINVERTEX);
	UINT offset = 0;
	devCon11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット
	devCon11->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット
	devCon11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CameraData cam;
	ZeroMemory(&cam, sizeof(CameraData));
	bool isSuccess = TellCameraData::GetCameraData(&cam, CAMERA_NUMBER::CAMERA_0);

	if (!isSuccess)
	{
		// エラーメッセージ
		MyOutputDebugString("カメラ取得失敗");
	}

	//カメラ位置をシェーダーに渡す
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBufferLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_SKIN_GLOBAL0 sg;
			sg.vLightDir = D3DXVECTOR4(1, 1, -1, 0.0f);
			sg.vEye = D3DXVECTOR4(cam.pos.x, cam.pos.y, cam.pos.z, 0);
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL0));
			Direct3D11::GetDeviceContext()->Unmap(m_pConstantBufferLight, 0);
		}
		devCon11->VSSetConstantBuffers(0, 1, &m_pConstantBufferLight);
		devCon11->PSSetConstantBuffers(0, 1, &m_pConstantBufferLight);
	}


	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		devCon11->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//マテリアルの各要素と変換行列をシェーダーに渡す			
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devCon11->Map(m_pConstantBufferWVPMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_SKIN_GLOBAL1 sg;
			sg.mW = world;
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			sg.mWVP = world * cam.matrixView * cam.matrixProj;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			sg.vAmbient = m_pMaterial[i].Ka;
			sg.vDiffuse = m_pMaterial[i].Kd;
			sg.vSpecular = m_pMaterial[i].Ks;
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL1));
			devCon11->Unmap(m_pConstantBufferWVPMaterial, 0);
		}
		devCon11->VSSetConstantBuffers(1, 1, &m_pConstantBufferWVPMaterial);
		devCon11->PSSetConstantBuffers(1, 1, &m_pConstantBufferWVPMaterial);
		//テクスチャーをシェーダーに渡す
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			devCon11->PSSetSamplers(0, 1, &m_pSampler);
			devCon11->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			devCon11->PSSetShaderResources(0, 1, Nothing);
		}
		//Draw
		devCon11->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}

	//アニメーションフレームを進める　スキンを更新
	SetNewPoseMatrices();

	//アニメ進行
	if (m_pMeshParser->m_pAnimController)
	{
		m_pMeshParser->m_pAnimController->AdvanceTime(0.01666, NULL);
	}
	D3DXMATRIX m /*= world * cam.matrixView * cam.matrixProj*/;
	D3DXMatrixIdentity(&m);
	m_pMeshParser->UpdateFrameMatrices(m_pMeshParser->m_pFrameRoot, &m);
}

/**
* @fn Init
* @breif 初期化
* @param[in] fileName モデルのファイル名
* @return HRESULT S_OKで成功
*/
HRESULT SkinMesh::Init(LPCSTR fileName)
{
	auto hr = CreateFromX(fileName);
	//D3D11関連の初期化
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	//ブロブからバーテックスシェーダー作成
	if (FAILED(D3DX11CompileFromFile("Shader/Geometry_Material_Texture_Skin.hlsl", NULL, NULL, "VSSkin", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		int size = pErrors->GetBufferSize();
		char* ch = (char*)pErrors->GetBufferPointer();
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
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//頂点インプットレイアウトを作成
	if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return E_FAIL;
	//頂点インプットレイアウトをセット
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//ブロブからピクセルシェーダー作成
	if (FAILED(D3DX11CompileFromFile("Shader/Geometry_Material_Texture_Skin.hlsl", NULL, NULL, "PSSkin", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
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

	D3D11_BUFFER_DESC cb;
	//コンスタントバッファー0作成
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferLight)))
	{
		return E_FAIL;
	}
	//コンスタントバッファー1作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferWVPMaterial)))
	{
		return E_FAIL;
	}
	//コンスタントバッファーボーン用　作成  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL_BONES);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferBone)))
	{
		return E_FAIL;
	}
	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampler);

	return S_OK;
}

/**
* @fn ReadSkinInfo
* @brief xfileからスキン関連の情報を読み出す
* @param[in] pvVB 頂点データの構造体のポインタ
* @return HRESULT S_OKで成功
*/
HRESULT SkinMesh::ReadSkinInfo(MY_SKINVERTEX* pvVB)
{
	//Xから抽出すべき情報は、頂点ごとのボーンインデックス、頂点ごとのボーンウェイト、バインド行列、ポーズ行列　の4項目

	int i, k, m, n;
	int iNumVertex = m_pMeshParser->GetNumVertices();//頂点数
	int iNumBone = 0;//ボーン数


	//ボーン	の個数を得る
	iNumBone = m_pMeshParser->GetNumBones();
	//それぞれのボーンに影響を受ける頂点を調べる　そこから逆に、頂点ベースでボーンインデックス・重みを整頓する
	for (i = 0; i < iNumBone; i++)
	{
		int iNumIndex = m_pMeshParser->GetNumBoneVertices(i);//このボーンに影響を受ける頂点数
		int* piIndex = new int[iNumIndex];
		for (k = 0; k < iNumIndex; k++) piIndex[k] = m_pMeshParser->GetBoneVerticesIndices(i, k);
		double* pdWeight = new double[iNumIndex];
		for (k = 0; k < iNumIndex; k++) pdWeight[k] = m_pMeshParser->GetBoneVerticesWeights(i, k);
		//頂点側からインデックスをたどって、頂点サイドで整理する
		for (k = 0; k < iNumIndex; k++)
		{
			//XやCGソフトがボーン4本以内とは限らない。5本以上の場合は、重みの大きい順に4本に絞る 

			//ウェイトの大きさ順にソート（バブルソート）
			for (m = 4; m > 1; m--)
			{
				for (n = 1; n < m; n++)
				{
					if (pvVB[piIndex[k]].bBoneWeight[n - 1] < pvVB[piIndex[k]].bBoneWeight[n])
					{
						float tmp = pvVB[piIndex[k]].bBoneWeight[n - 1];
						pvVB[piIndex[k]].bBoneWeight[n - 1] = pvVB[piIndex[k]].bBoneWeight[n];
						pvVB[piIndex[k]].bBoneWeight[n] = tmp;
						int itmp = pvVB[piIndex[k]].bBoneIndex[n - 1];
						pvVB[piIndex[k]].bBoneIndex[n - 1] = pvVB[piIndex[k]].bBoneIndex[n];
						pvVB[piIndex[k]].bBoneIndex[n] = itmp;
					}
				}
			}
			//ソート後は、最後の要素に一番小さいウェイトが入っているはず。
			bool flag = false;
			for (m = 0; m < 4; m++)
			{
				if (pvVB[piIndex[k]].bBoneWeight[m] == 0)
				{
					flag = true;
					pvVB[piIndex[k]].bBoneIndex[m] = i;
					pvVB[piIndex[k]].bBoneWeight[m] = pdWeight[k];
					break;
				}
			}
			if (flag == false)
			{
				pvVB[piIndex[k]].bBoneIndex[3] = i;
				pvVB[piIndex[k]].bBoneWeight[3] = pdWeight[k];
				break;
			}

		}
		delete piIndex;
		delete pdWeight;
	}
	//
	//ボーンを生成
	m_iNumBone = iNumBone;
	m_BoneArray = new BONE[iNumBone];
	//ポーズ行列を得る 初期ポーズ
	for (i = 0; i < m_iNumBone; i++)
	{
		m_BoneArray[i].mBindPose = m_pMeshParser->GetBindPose(i);
	}

	return S_OK;
}

/**
* @fn CreateFromX
* @brief xfileからスキンメッシュを作成する
* @param[in] szFileName モデルのファイル名
* @return HRESULT S_OKで成功
*/
HRESULT SkinMesh::CreateFromX(LPCSTR szFileName)
{
	//Xファイル読み込み
	m_pMeshParser = new D3DXPARSER;
	m_pMeshParser->LoadMeshFromX(Direct3D9::GetDevice(), szFileName);

	//事前に頂点数、ポリゴン数等を調べる
	DWORD dwNumVert = m_pMeshParser->GetNumVertices();
	DWORD dwNumFace = m_pMeshParser->GetNumFaces();
	DWORD dwNumUV = m_pMeshParser->GetNumUVs();
	if (dwNumVert < dwNumUV)//Direct3DではUVの数だけ頂点が必要
	{
		//共有頂点等で、頂点数が足りない時
		MessageBox(0, "Direct3Dは、UVの数だけ頂点が必要です（UVを置く場所が必要です）テクスチャーは正しく貼られないと思われます", NULL, MB_OK);
		return E_FAIL;
	}
	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[dwNumVert];
	int* piFaceBuffer = new int[dwNumFace * 3];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

	//頂点読み込み
	for (int i = 0; i < dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pMeshParser->GetVertexCoord(i);
		pvVB[i].vPos.x = v.x;
		pvVB[i].vPos.y = v.y;
		pvVB[i].vPos.z = v.z;
	}
	//ポリゴン情報（頂点インデックス）読み込み
	for (int i = 0; i < dwNumFace * 3; i++)
	{
		piFaceBuffer[i] = m_pMeshParser->GetIndex(i);
	}
	//法線読み込み
	for (int i = 0; i < dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pMeshParser->GetNormal(i);
		pvVB[i].vNorm.x = v.x;
		pvVB[i].vNorm.y = v.y;
		pvVB[i].vNorm.z = v.z;
	}
	//テクスチャー座標読み込み
	for (int i = 0; i < dwNumVert; i++)
	{
		D3DXVECTOR2 v = m_pMeshParser->GetUV(i);
		pvVB[i].vTex.x = v.x;
		pvVB[i].vTex.y = v.y;
	}
	//マテリアル読み込み
	m_dwNumMaterial = m_pMeshParser->GetNumMaterials();
	m_pMaterial = new MY_SKINMATERIAL[m_dwNumMaterial];

	//マテリアルの数だけインデックスバッファーを作成
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];
	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//環境光	
		m_pMaterial[i].Ka.x = m_pMeshParser->GetAmbient(i).y;
		m_pMaterial[i].Ka.y = m_pMeshParser->GetAmbient(i).z;
		m_pMaterial[i].Ka.z = m_pMeshParser->GetAmbient(i).w;
		m_pMaterial[i].Ka.w = m_pMeshParser->GetAmbient(i).x;
		//拡散反射光	
		m_pMaterial[i].Kd.x = m_pMeshParser->GetDiffuse(i).y;
		m_pMaterial[i].Kd.y = m_pMeshParser->GetDiffuse(i).z;
		m_pMaterial[i].Kd.z = m_pMeshParser->GetDiffuse(i).w;
		m_pMaterial[i].Kd.w = m_pMeshParser->GetDiffuse(i).x;
		//鏡面反射光
		m_pMaterial[i].Ks.x = m_pMeshParser->GetSpecular(i).y;
		m_pMaterial[i].Ks.y = m_pMeshParser->GetSpecular(i).z;
		m_pMaterial[i].Ks.z = m_pMeshParser->GetSpecular(i).w;
		m_pMaterial[i].Ks.w = m_pMeshParser->GetSpecular(i).x;

		//テクスチャー（ディフューズテクスチャーのみ）
		char* name = m_pMeshParser->GetTexturePath(i);
		if (name)
		{
			strcpy(m_pMaterial[i].szTextureName, name);
		}
		//テクスチャーを作成
		if (m_pMaterial[i].szTextureName[0] != 0 && FAILED(D3DX11CreateShaderResourceViewFromFileA(Direct3D11::GetDevice(), m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))//絶対パスファイル名は、まず失敗すると思うが、、、
		{
			MessageBox(0, "テクスチャー読み込み失敗", NULL, MB_OK);
			return E_FAIL;
		}

		//そのマテリアルであるインデックス配列内の開始インデックスを調べる　さらにインデックスの個数も調べる
		int iCount = 0;
		int* pIndex = new int[dwNumFace * 3];//とりあえず、メッシュ内のポリゴン数でメモリ確保（個々のポリゴングループはかならずこれ以下になるが）

		for (int k = 0; k < dwNumFace; k++)
		{
			if (i == m_pMeshParser->GeFaceMaterialIndex(k))//もし i == k番目のポリゴンのマテリアル番号 なら
			{
				pIndex[iCount] = m_pMeshParser->GetFaceVertexIndex(k, 0);//k番目のポリゴンを作る頂点のインデックス　1個目
				pIndex[iCount + 1] = m_pMeshParser->GetFaceVertexIndex(k, 1);//2個目
				pIndex[iCount + 2] = m_pMeshParser->GetFaceVertexIndex(k, 2);//3個目
				iCount += 3;
			}
		}
		if (iCount > 0) CreateIndexBuffer(iCount * sizeof(int), pIndex, &m_ppIndexBuffer[i]);
		delete pIndex;
		m_pMaterial[i].dwNumFace = iCount / 3;//そのマテリアル内のポリゴン数

	}
	//スキン情報（ジョイント、ウェイト　）読み込み
	ReadSkinInfo(pvVB);
	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_SKINVERTEX) * dwNumVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return E_FAIL;

	//一時的な入れ物は、もはや不要
	delete piFaceBuffer;
	if (pvVB) delete pvVB;

	return S_OK;
}

/**
* @fn CreateIndexBuffer
* @brief インデックスバッファー作成
* @param[in] dwSize バッファーサイズ
* @param[in] pIndex 初期化データへのポインター
* @param[out, optional] ppIndexBuffer 作成されたバッファーオブジェクトのID3D11Bufferインターフェイスへのポインターのアドレス
* @return HRESULT S_OKで成功
*/
HRESULT SkinMesh::CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dwSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, ppIndexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/**
* @fn SetNewPoseMatrices
* @brief ボーンを次のポーズ位置にセットする
* @return HRESULT S_OKで成功
*/
void SkinMesh::SetNewPoseMatrices()
{
	//望むフレームでUpdateすること。しないと行列が更新されない
	//m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot)をレンダリング時に実行すること

	//また、アニメーション時間に見合った行列を更新するのはD3DXMESHでは
	//アニメーションコントローラーが（裏で）やってくれるものなので、アニメーションコントローラー
	//を使ってアニメを進行させることも必要
	//	m_pD3dxMesh->m_pAnimController->AdvanceTime(....)をレンダリング時に実行すること

	for (int i = 0; i < m_iNumBone; i++)
	{
		m_BoneArray[i].mNewPose = m_pMeshParser->GetNewPose(i);
	}
}

/**
* @fn GetCurrentPoseMatrix
* @brief 次の(現在の)ポーズ行列を返す
* @param[in] index ボーン配列の添え字
* @return D3DXMATRIX 次の(現在の)ポーズ行列
*/
D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
{
	D3DXMATRIX ret = m_BoneArray[index].mBindPose * m_BoneArray[index].mNewPose;
	return ret;
}

/**
* @fn GetBindPoseMatrix
* @brief バインドポーズ行列を返す
* @param[in] index ボーン配列の添え字
* @return D3DXMATRIX バインドポーズ行列
*/
D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
{
	return m_BoneArray[index].mBindPose;
}

/**
* @fn GetBoneNames
* @brief ボーンの名前取得
* @param[in] iBoneIndex ボーン配列の添え字
* @return CHAR* ボーンの名前
*/
CHAR* SkinMesh::GetBoneNames(int iBoneIndex)
{
	return m_pMeshParser->GetBoneName(iBoneIndex);
}
