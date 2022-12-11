#pragma once

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <d3dCompiler.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

#define SAFE_RELEASE(x) if(x){x->Release(); x=0;}
#define SAFE_DELETE(x) if(x){delete x; x=0;}
#define SAFE_DELETE_ARRAY(x) if(x){delete[] x; x=0;}

//頂点の構造体
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};
//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLECONSTANT_BUFFER0
{
	D3DXMATRIX mW;//ワールド行列
	D3DXMATRIX mWVP;//ワールドから射影までの変換行列
	D3DXVECTOR4 vLightDir;//ライト方向
	D3DXVECTOR4 vEye;//カメラ位置
};

struct SIMPLECONSTANT_BUFFER1
{
	D3DXVECTOR4 vAmbient;//アンビエント光
	D3DXVECTOR4 vDiffuse;//ディフューズ色
	D3DXVECTOR4 vSpecular;//鏡面反射
};
//オリジナル　マテリアル構造体
struct MY_MATERIAL
{
	CHAR szName[110];
	D3DXVECTOR4 Ka;//アンビエント
	D3DXVECTOR4 Kd;//ディフューズ
	D3DXVECTOR4 Ks;//スペキュラー
	CHAR szTextureName[110];//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;//そのマテリアルであるポリゴン数
	MY_MATERIAL()
	{
		ZeroMemory(this,sizeof(MY_MATERIAL));
	}
	~MY_MATERIAL()
	{
		SAFE_RELEASE(pTexture);
	}
};

class ObjMesh
{
public:
	ObjMesh();
	~ObjMesh();
	HRESULT Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LPCSTR FileName);
	HRESULT InitShader();
	HRESULT LoadMaterialFromFile(LPCSTR FileName, MY_MATERIAL** ppMaterial);
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye);

	DWORD m_dwNumVert;
	DWORD m_dwNumFace;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;

	DWORD m_dwNumMaterial;
	MY_MATERIAL* m_pMaterial;
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11ShaderResourceView* m_pTexture;

	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	HRESULT LoadStaticMesh(LPCSTR FileName);
};
