/**
* @file StaticMesh.h
* @brief StaticMeshの描画
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// ヘッダーファイルのインクルード
#pragma once
#include <d3dx9.h>
#include <d3d11.h>
#include "Game/Component/Component.h"
#include "Game/Component/Transform/Transform.h"
#include "System/Common.h"

// using宣言
using math::Vector3D;

struct ConstantBuffer0
{
	D3DXMATRIX world;//ワールド行列
	D3DXMATRIX worldVewProj;//ワールドから射影までの変換行列
	D3DXVECTOR4 lightDir;//ライト方向
	D3DXVECTOR4 eye;//カメラ位置
};

struct ConstantBuffer1
{
	D3DXVECTOR4 ambient;//アンビエント光
	D3DXVECTOR4 diffuse;//ディフューズ色
	D3DXVECTOR4 specular;//鏡面反射
};

//オリジナル　マテリアル構造体
struct MyMaterial
{
	CHAR name[110];
	D3DXVECTOR4 ambient;//アンビエント
	D3DXVECTOR4 diffuse;//ディフューズ
	D3DXVECTOR4 specular;//スペキュラー
	CHAR textureName[110];//テクスチャーファイル名
	ID3D11ShaderResourceView* pTexture;
	DWORD numFace;//そのマテリアルであるポリゴン数
	MyMaterial()
	{
		ZeroMemory(this, sizeof(MyMaterial));
	}
	~MyMaterial()
	{
		SAFE_RELEASE(pTexture);
	}
};

//頂点の構造体
struct MY_VERTEX
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;
};

/**
* @class StaticMesh
* @brief 動かないメッシュクラス
*/
class StaticMesh : public Component
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
	* @fn Init
	* @brief 初期化
	*/
	HRESULT Init(const char* fileName);

private:
	/**
	* @fn LoadXMesh
	* @brief xfileをロードする
	* @param[in] fileName ファイルの名前
	* @return HRESULT S_OKで成功
	*/
	HRESULT LoadXMesh(const char* fileName);


	Transform* m_transform; // トランスフォーム
	LPD3DXMESH m_pMesh; // メッシュ
	DWORD m_numMaterial; // マテリアルの数

	DWORD m_numAttribute; // 属性数
	DWORD m_attributeID[300]; // 300属性まで

	// resource
	char m_textureFileName[8][256];//テクスチャーファイル名（８枚まで）
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	MyMaterial* m_pMaterial;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;

	bool m_useTexture;
};
