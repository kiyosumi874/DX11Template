#include "ObjMesh.h"
#include <D3D11/Direct3D11.h>
#include <D3D11/ShaderDirector/ShaderDirector.h>
#include <System/Transform.h>
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>

//
//
//
ObjMesh::ObjMesh()
{
	ZeroMemory(this,sizeof(ObjMesh));
	m_fScale=1.0f;
}

//
//
//
ObjMesh::~ObjMesh()
{
	SAFE_DELETE_ARRAY(m_pMaterial);
	SAFE_DELETE_ARRAY(m_ppIndexBuffer);
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pTexture);
}

//
//
//
HRESULT ObjMesh::Init(ID3D11Device* pDevice,ID3D11DeviceContext* pContext,LPCSTR FileName)
{
	m_pDevice=pDevice;
	m_pDeviceContext=pContext;

	if(FAILED(InitShader()))
	{
		MessageBox(0,"メッシュ用シェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	if(FAILED(LoadStaticMesh(FileName)))
	{
		MessageBox(0,"メッシュ作成失敗",NULL,MB_OK);
		return E_FAIL;
	}

	return S_OK;
}
//
//
//
HRESULT ObjMesh::InitShader()
{
	//hlslファイル読み込み ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3D10Blob *pCompiledShader=NULL;
	ID3D10Blob *pErrors=NULL;
	//ブロブからバーテックスシェーダー作成
	if(FAILED(D3DX11CompileFromFile("Shader/MESH2.hlsl",NULL,NULL,"VS","vs_4_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if(FAILED(m_pDevice->CreateVertexShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,"バーテックスシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout)/sizeof(layout[0]);
	//頂点インプットレイアウトを作成
	if( FAILED( m_pDevice->CreateInputLayout( layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		return FALSE;
	}
	//ブロブからピクセルシェーダー作成
	if(FAILED(D3DX11CompileFromFile("Shader/MESH2.hlsl",NULL,NULL,"PS","ps_4_0",0,0,NULL,&pCompiledShader,&pErrors,NULL)))
	{
		MessageBox(0,"hlsl読み込み失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if(FAILED(m_pDevice->CreatePixelShader(pCompiledShader->GetBufferPointer(),pCompiledShader->GetBufferSize(),NULL,&m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0,"ピクセルシェーダー作成失敗",NULL,MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);
	//コンスタントバッファー作成　変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth= sizeof( SIMPLECONSTANT_BUFFER0 );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	=0;
	cb.Usage=D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice->CreateBuffer( &cb,NULL,&m_pConstantBuffer0)))
	{
		return E_FAIL;
	}
	//コンスタントバッファー作成  マテリアル渡し用
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth= sizeof( SIMPLECONSTANT_BUFFER1 );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags	=0;
	cb.Usage=D3D11_USAGE_DYNAMIC;

	if( FAILED(m_pDevice->CreateBuffer( &cb,NULL,&m_pConstantBuffer1)))
	{
		return E_FAIL;
	}

	return S_OK;
}
//
//
//
HRESULT ObjMesh::LoadMaterialFromFile(LPCSTR FileName,MY_MATERIAL** ppMaterial)
{
	//マテリアルファイルを開いて内容を読み込む
	FILE* fp=NULL;
	fopen_s(&fp,FileName,"rt");
	if (fp == NULL)
	{
		return E_FAIL;
	}
	char key[110]={0};
	D3DXVECTOR4 v(0,0,0,1);

	//マテリアル数を調べる
	m_dwNumMaterial=0;
	while(!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp,"%s ",key,sizeof(key));
		//マテリアル名
		if(strcmp(key,"newmtl")==0)
		{
			m_dwNumMaterial++;
		}
	}
	MY_MATERIAL* pMaterial=new MY_MATERIAL[m_dwNumMaterial];
	
	//本読み込み	
	fseek(fp,SEEK_SET,0);
	INT iMCount=-1;

	while(!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp,"%s ",key,sizeof(key));
		//マテリアル名
		if(strcmp(key,"newmtl")==0)
		{
			iMCount++;
			fscanf_s(fp,"%s ",key,sizeof(key));
			strcpy_s(pMaterial[iMCount].szName,key);			
		}
		//Ka　アンビエント
		if(strcmp(key,"Ka")==0)
		{
			fscanf_s(fp,"%f %f %f",&v.x,&v.y,&v.z);
			pMaterial[iMCount].Ka=v;
		}
		//Kd　ディフューズ
		if(strcmp(key,"Kd")==0)
		{
			fscanf_s(fp,"%f %f %f",&v.x,&v.y,&v.z);
			pMaterial[iMCount].Kd=v;
		}
		//Ks　スペキュラー
		if(strcmp(key,"Ks")==0)
		{
			fscanf_s(fp,"%f %f %f",&v.x,&v.y,&v.z);
			pMaterial[iMCount].Ks=v;
		}
		//map_Kd　テクスチャー
		if(strcmp(key,"map_Kd")==0)
		{
			fscanf_s(fp,"%s",&pMaterial[iMCount].szTextureName,sizeof(pMaterial[iMCount].szTextureName));	
			//テクスチャーを作成
			if(FAILED(D3DX11CreateShaderResourceViewFromFileA( m_pDevice, pMaterial[iMCount].szTextureName, NULL, NULL, &pMaterial[iMCount].pTexture, NULL )))
			{
				return E_FAIL;
			}
		}
	}
	fclose(fp);

	*ppMaterial=pMaterial;
	
	return S_OK;
}
//
//
//
HRESULT ObjMesh::LoadStaticMesh(LPCSTR FileName)
{
	float x,y,z;
	int v1=0,v2=0,v3=0;
	int vn1=0,vn2=0,vn3=0;
	int vt1=0,vt2=0,vt3=0;
	DWORD dwVCount=0;//読み込みカウンター
	DWORD dwVNCount=0;//読み込みカウンター
	DWORD dwVTCount=0;//読み込みカウンター
	DWORD dwFCount=0;//読み込みカウンター

	char key[200]={0};
	//OBJファイルを開いて内容を読み込む
	FILE* fp=NULL;
	fopen_s(&fp,FileName,"rt");

	//事前に頂点数、ポリゴン数を調べる
	while(!feof(fp))
	{
		//キーワード読み込み
		fscanf_s(fp,"%s ",key,sizeof(key));
		//マテリアル読み込み
		if(strcmp(key,"mtllib")==0)
		{
			fscanf_s(fp,"%s ",key,sizeof(key));
			if (FAILED(LoadMaterialFromFile(key, &m_pMaterial)))
			{
				MessageBox(0, "マテリアルがないです", NULL, MB_OK);
				return E_FAIL;
			}
		}					
		//頂点
		if(strcmp(key,"v")==0)
		{
			m_dwNumVert++;
		}
		//法線
		if(strcmp(key,"vn")==0)
		{
			dwVNCount++;
		}
		//テクスチャー座標
		if(strcmp(key,"vt")==0)
		{
			dwVTCount++;
		}
		//フェイス（ポリゴン）
		if(strcmp(key,"f")==0)
		{
			m_dwNumFace++;
		}
	}

	//一時的なメモリ確保（頂点バッファとインデックスバッファ）
	MY_VERTEX* pvVertexBuffer =new MY_VERTEX[ m_dwNumFace*3 ];
	D3DXVECTOR3* pvCoord=new D3DXVECTOR3[ m_dwNumVert ];
	D3DXVECTOR3* pvNormal=new D3DXVECTOR3[ dwVNCount ];
	D3DXVECTOR2* pvTexture=new D3DXVECTOR2[ dwVTCount];
	
	//本読み込み	
	fseek(fp,SEEK_SET,0);
	dwVCount=0;
	dwVNCount=0;
	dwVTCount=0;
	dwFCount=0;	

	while(!feof(fp))
	{
		//キーワード 読み込み
		ZeroMemory(key,sizeof(key));
		fscanf_s(fp,"%s ",key,sizeof(key));

		//頂点 読み込み
		if(strcmp(key,"v")==0)
		{		
			fscanf_s(fp,"%f %f %f",&x,&y,&z);
			pvCoord[dwVCount].x=-x;
			pvCoord[dwVCount].y=y;
			pvCoord[dwVCount].z=z;
			dwVCount++;
		}

		//法線 読み込み
		if(strcmp(key,"vn")==0)
		{		
			fscanf_s(fp,"%f %f %f",&x,&y,&z);
			pvNormal[dwVNCount].x=-x;
			pvNormal[dwVNCount].y=y;
			pvNormal[dwVNCount].z=z;
			dwVNCount++;
		}

		//テクスチャー座標 読み込み
		if(strcmp(key,"vt")==0)
		{		
			fscanf_s(fp,"%f %f",&x,&y);
			pvTexture[dwVTCount].x=x;
			pvTexture[dwVTCount].y=1-y;//OBJファイルはY成分が逆なので合わせる
			dwVTCount++;
		}		
	}	

	//マテリアルの数だけインデックスバッファーを作成
	m_ppIndexBuffer=new ID3D11Buffer*[m_dwNumMaterial];

	//フェイス　読み込み　バラバラに収録されている可能性があるので、マテリアル名を頼りにつなぎ合わせる
	bool boFlag=false;
	int* piFaceBuffer=new int[ m_dwNumFace*3 ];//３頂点ポリゴンなので、1フェイス=3頂点(3インデックス)
	dwFCount=0;
	DWORD dwPartFCount=0;
	for(DWORD i=0;i<m_dwNumMaterial;i++)
	{		
		dwPartFCount=0;
		fseek(fp,SEEK_SET,0);		

		while(!feof(fp))
		{
			//キーワード 読み込み
			ZeroMemory(key,sizeof(key));
			fscanf_s(fp,"%s ",key,sizeof(key));

			//フェイス 読み込み→頂点インデックスに
			if(strcmp(key,"usemtl")==0)
			{
				fscanf_s(fp,"%s ",key,sizeof(key));
				if(strcmp(key,m_pMaterial[i].szName)==0)
				{
					boFlag=true;
				}
				else
				{
					 boFlag=false;
				}
			}
			if(strcmp(key,"f")==0 && boFlag==true)
			{
				if(m_pMaterial[i].pTexture!=NULL)//テクスチャーありサーフェイス
				{
					fscanf_s(fp,"%d/%d/%d %d/%d/%d %d/%d/%d",&v1,&vt1,&vn1,&v2,&vt2,&vn2,&v3,&vt3,&vn3);
				}
				else//テクスチャー無しサーフェイス
				{
					fscanf_s(fp,"%d//%d %d//%d %d//%d",&v1,&vn1,&v2,&vn2,&v3,&vn3);
				}

				//インデックスバッファー
				piFaceBuffer[dwPartFCount*3]=dwFCount*3;
				piFaceBuffer[dwPartFCount*3+1]=dwFCount*3+1;
				piFaceBuffer[dwPartFCount*3+2]=dwFCount*3+2;
				//頂点構造体に代入
				pvVertexBuffer[dwFCount*3].vPos=pvCoord[v1-1];
				pvVertexBuffer[dwFCount*3].vNorm=pvNormal[vn1-1];
				pvVertexBuffer[dwFCount*3].vTex=pvTexture[vt1-1];
				pvVertexBuffer[dwFCount*3+1].vPos=pvCoord[v2-1];
				pvVertexBuffer[dwFCount*3+1].vNorm=pvNormal[vn2-1];
				pvVertexBuffer[dwFCount*3+1].vTex=pvTexture[vt2-1];
				pvVertexBuffer[dwFCount*3+2].vPos=pvCoord[v3-1];
				pvVertexBuffer[dwFCount*3+2].vNorm=pvNormal[vn3-1];
				pvVertexBuffer[dwFCount*3+2].vTex=pvTexture[vt3-1];

				dwPartFCount++;
				dwFCount++;
		
			}
		}
		if(dwPartFCount==0)//使用されていないマテリアル対策
		{
			m_ppIndexBuffer[i]=NULL;
			continue;
		}

		//インデックスバッファーを作成
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( int ) * dwPartFCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = piFaceBuffer;
		if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &m_ppIndexBuffer[i] ) ) )
			return FALSE;
		m_pMaterial[i].dwNumFace=dwPartFCount;
	}
	delete[] piFaceBuffer;
	fclose(fp);

	//バーテックスバッファーを作成
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( MY_VERTEX ) *m_dwNumFace*3;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVertexBuffer;

	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer ) ) )
		return FALSE;

	//一時的な入れ物は、もはや不要
	delete pvCoord;
	delete pvNormal;
	delete pvTexture;
	delete[] pvVertexBuffer;

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc,sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    m_pDevice->CreateSamplerState( &SamDesc, &m_pSampleLinear);

	return S_OK;
}

//
//
//
void ObjMesh::Render(D3DXMATRIX& mView,D3DXMATRIX& mProj,
				  D3DXVECTOR3& vLight,D3DXVECTOR3& vEye)
{
	D3DXMATRIX mWorld,mTran,mYaw,mPitch,mRoll,mScale;
	//ワールドトランスフォーム（絶対座標変換）
	D3DXMatrixScaling(&mScale,m_fScale,m_fScale,m_fScale);
	D3DXMatrixRotationY(&mYaw,m_fYaw);
	D3DXMatrixRotationX(&mPitch,m_fPitch);
	D3DXMatrixRotationZ(&mRoll,m_fRoll);
	D3DXMatrixTranslation(&mTran,m_vPos.x,m_vPos.y,m_vPos.z);

	mWorld=mScale*mYaw*mPitch*mRoll*mTran;
	//使用するシェーダーの登録	
	m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);
	m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);
	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		SIMPLECONSTANT_BUFFER0 sg;
		//ワールド行列を渡す
		sg.mW=mWorld;
		D3DXMatrixTranspose( &sg.mW, &sg.mW );
		//ワールド、カメラ、射影行列を渡す
		sg.mWVP=mWorld*mView*mProj;
		D3DXMatrixTranspose( &sg.mWVP, &sg.mWVP );
		//ライトの方向を渡す
		sg.vLightDir=D3DXVECTOR4(vLight.x,vLight.y,vLight.z,0.0f);
		//視点位置を渡す
		sg.vEye=D3DXVECTOR4(vEye.x,vEye.y,vEye.z,0);

		memcpy_s( pData.pData, pData.RowPitch, (void*)&sg, sizeof( SIMPLECONSTANT_BUFFER0 ) );
		m_pDeviceContext->Unmap( m_pConstantBuffer0, 0 );
	}
	//このコンスタントバッファーを使うシェーダーの登録
	m_pDeviceContext->VSSetConstantBuffers(0,1,&m_pConstantBuffer0 );
	m_pDeviceContext->PSSetConstantBuffers(0,1,&m_pConstantBuffer0 );
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );
	//プリミティブ・トポロジーをセット
	m_pDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	//バーテックスバッファーをセット
	UINT stride = sizeof( MY_VERTEX );
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for(DWORD i=0;i<m_dwNumMaterial;i++)
	{
		//使用されていないマテリアル対策
		if(m_pMaterial[i].dwNumFace==0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof( int );
		offset = 0;
		m_pDeviceContext->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0 );
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if( SUCCEEDED( m_pDeviceContext->Map( m_pConstantBuffer1,0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
		{
			SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient=m_pMaterial[i].Ka;//アンビエントををシェーダーに渡す
			sg.vDiffuse=m_pMaterial[i].Kd;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular=m_pMaterial[i].Ks;//スペキュラーをシェーダーに渡す
			memcpy_s( pData.pData, pData.RowPitch, (void*)&sg, sizeof( SIMPLECONSTANT_BUFFER1 ) );
			m_pDeviceContext->Unmap( m_pConstantBuffer1, 0 );
		}
		m_pDeviceContext->VSSetConstantBuffers(1,1,&m_pConstantBuffer1 );
		m_pDeviceContext->PSSetConstantBuffers(1,1,&m_pConstantBuffer1 );
		//テクスチャーをシェーダーに渡す
		if(m_pMaterial[i].szTextureName[0] != NULL)
		{
			m_pDeviceContext->PSSetSamplers(0,1,&m_pSampleLinear);
			m_pDeviceContext->PSSetShaderResources(0,1,&m_pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1]={0};
			m_pDeviceContext->PSSetShaderResources(0,1,Nothing);
		}
		//プリミティブをレンダリング
		m_pDeviceContext->DrawIndexed(m_pMaterial[i].dwNumFace*3 , 0 ,0);
	}

}

void ObjMesh::Draw(const Transform& transform)
{
	UINT mask = 0xffffffff;
	Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, mask);

	

	//使用するシェーダーのセット
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);

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
		SIMPLECONSTANT_BUFFER0 sg;
		//ワールド行列を渡す
		sg.mW = world;
		D3DXMatrixTranspose(&sg.mW, &sg.mW);
		//ワールド、カメラ、射影行列を渡す
		sg.mWVP = world * data.matrixView * data.matrixProj;
		D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
		//ライトの方向を渡す
		auto vLight = D3DXVECTOR3(1, 1, -1);
		sg.vLightDir = D3DXVECTOR4(vLight.x, vLight.y, vLight.z, 0.0f);
		//視点位置を渡す
		sg.vEye = D3DXVECTOR4(data.pos.x, data.pos.y, data.pos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER0));
		m_pDeviceContext->Unmap(m_pConstantBuffer0, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	//頂点インプットレイアウトをセット
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//バーテックスバッファーをセット
	UINT stride = sizeof(MY_VERTEX);
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//マテリアルの数だけ、それぞれのマテリアルのインデックスバッファ－を描画
	for (DWORD i = 0; i < m_dwNumMaterial; i++)
	{
		//使用されていないマテリアル対策
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//インデックスバッファーをセット
		stride = sizeof(int);
		offset = 0;
		m_pDeviceContext->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//マテリアルの各要素をエフェクト（シェーダー）に渡す
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(m_pDeviceContext->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SIMPLECONSTANT_BUFFER1 sg;
			sg.vAmbient = m_pMaterial[i].Ka;//アンビエントををシェーダーに渡す
			sg.vDiffuse = m_pMaterial[i].Kd;//ディフューズカラーをシェーダーに渡す
			sg.vSpecular = m_pMaterial[i].Ks;//スペキュラーをシェーダーに渡す
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SIMPLECONSTANT_BUFFER1));
			m_pDeviceContext->Unmap(m_pConstantBuffer1, 0);
		}
		m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		m_pDeviceContext->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		//テクスチャーをシェーダーに渡す
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			m_pDeviceContext->PSSetSamplers(0, 1, &m_pSampleLinear);
			m_pDeviceContext->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext->PSSetShaderResources(0, 1, Nothing);
		}
		//プリミティブをレンダリング
		m_pDeviceContext->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
}
