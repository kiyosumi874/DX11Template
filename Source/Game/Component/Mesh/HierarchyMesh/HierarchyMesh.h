/**
* @file HierarchyMesh.h
* @brief HierarchyMeshの描画(まだ完全ではない)
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// ヘッダーファイルのインクルード
#pragma once
#include <d3dx9.h>
#include <d3d11.h>
#include "Game/Component/Component.h"
#include "System/Transform.h"
#include "System/Common.h"
#include <vector>

// using宣言
using std::vector;

struct ConstantBufferMesh
{
	D3DXMATRIX world;//ワールド行列
	D3DXMATRIX worldVewProj;//ワールドから射影までの変換行列
	D3DXVECTOR4 lightDir;//ライト方向
	D3DXVECTOR4 eye;//カメラ位置
};

struct ConstantBufferMaterial
{
	D3DXVECTOR4 ambient;//アンビエント光
	D3DXVECTOR4 diffuse;//ディフューズ色
	D3DXVECTOR4 specular;//鏡面反射
};

//オリジナル　マテリアル構造体
struct MyHMaterial
{
	CHAR name[100];
	D3DXVECTOR4 ambient;//アンビエント
	D3DXVECTOR4 diffuse;//ディフューズ
	D3DXVECTOR4 specular;//スペキュラー
	CHAR textureName[256];//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	DWORD numFace;//そのマテリアルであるポリゴン数
	MyHMaterial()
	{
		ZeroMemory(this, sizeof(MyHMaterial));
	}
	~MyHMaterial()
	{
		SAFE_RELEASE(pTexture);
	}
};

//頂点の構造体
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

//パーツメッシュ　アプリ定義メッシュ(スタティックメッシュのサンプルにおけるCD3DXMESHクラスと同等）
struct PartsMesh
{
	DWORD numMaterial;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer** ppIndexBuffer;
	MyHMaterial* pMaterial;

	bool useTexture;
};
//派生フレーム構造体
struct MYHFRAME : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
	PartsMesh* pPartsMesh;
	MYHFRAME()
	{
		ZeroMemory(this, sizeof(MYHFRAME));
	}
};
//メッシュコンテナー構造体 フレーム内で使用
struct MYHMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures;
};
//Xファイル内のアニメーション階層を読み下すクラス。
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
* @brief 動かないメッシュクラス
*/
class HierarchyMesh : public Component
{
public:
	/**
	* @fn Start
	* @brief 生成したときに最初に一回だけ走る関数
	*/
	void Start() override;

	/**
	* @fn Update
	* @brief 更新
	*/
	void Update() override;

	/**
	* @fn Draw
	* @brief 描画
	*/
	void Draw() override;

	/**
	* @fn Terminate
	* @brief 終了処理
	*/
	void Terminate() override;

	/**
	* @fn Init
	* @brief 初期化
	*/
	HRESULT Init(const char* fileName);

	void ChangeAnimSet(int index);
private:
	/**
	* @fn LoadXMesh
	* @brief xfileをロードする
	* @param[in] fileName ファイルの名前
	* @return HRESULT S_OKで成功
	*/
	HRESULT LoadXAnimMesh(const char* fileName);
	HRESULT InitShader();

	void BuildAllMesh(D3DXFRAME* pFrame);
	HRESULT CreateAppMeshFromD3DXMesh(LPD3DXFRAME pFrame);
	void DrawFrame(LPD3DXFRAME p);
	void DrawPartsMesh(PartsMesh* p, D3DXMATRIX World);
	void UpdateHierarchyMatrices(D3DXFRAME* p, LPD3DXMATRIX pParentMatrix);

	//アニメ関連
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
	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	LPD3DXANIMATIONSET m_pAnimSet[100];//100個までのアニメーションセットに対応

};
