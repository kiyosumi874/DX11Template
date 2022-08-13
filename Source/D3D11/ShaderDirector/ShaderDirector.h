/**
* @file ShaderDirector.h
* @brief シェーダーの管理
* @details ShaderDirectorクラスはシングルトンなので注意
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// 多重インクルード防止
#pragma once
#include <D3D11.h>
#include <D3DX10.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include "D3D11/Direct3D11.h"
#include <System/Common.h>

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

// enumによる警告を消す
#pragma warning(disable:26812)



namespace D3D11
{
	/**
	* @fn CreateVertexBuffer
	*/
	template<typename T> static HRESULT CreateVertexBuffer(ID3D11Buffer** ppVertexBuffer, T* vertices, int vertexNum)
	{
		D3D11_BUFFER_DESC bd;
		bd.ByteWidth = sizeof(T) * vertexNum;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = vertices;

		return Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, ppVertexBuffer);
	}

	template<typename T> static void SetVertexBuffer(ID3D11Buffer** ppVertexBuffer)
	{
		UINT stride = sizeof(T);
		UINT offset = 0;
		Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, ppVertexBuffer, &stride, &offset);
	}

	static HRESULT CreateVertexShader(ID3D11VertexShader** ppVertexShader, ID3DBlob** ppCompiledShader, LPCSTR fileName)
	{
		auto& pCompiledShader = *ppCompiledShader;
		ID3DBlob* pErrors = NULL;

		if (FAILED(D3DX11CompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			char* p = (char*)pErrors->GetBufferPointer();
			MessageBoxA(0, p, 0, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);
		if (FAILED(Direct3D11::GetDevice()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, ppVertexShader)))
		{
			return E_FAIL;
		}
		return S_OK;
	}

	static HRESULT CreateInputLayout(ID3D11InputLayout** ppInputLayout, ID3DBlob** ppCompiledShader, D3D11_INPUT_ELEMENT_DESC* layoutArray, int elementCount)
	{
		auto& pCompiledShader = *ppCompiledShader;
		//頂点インプットレイアウトを作成
		if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layoutArray, elementCount, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), ppInputLayout)))
		{
			return E_FAIL;
		}
		return S_OK;
	}

	static HRESULT CreatePixelShader(ID3D11PixelShader** ppPixelShader, ID3DBlob** ppCompiledShader, LPCSTR fileName)
	{
		auto& pCompiledShader = *ppCompiledShader;
		ID3DBlob* pErrors = NULL;

		if (FAILED(D3DX11CompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pCompiledShader, &pErrors, NULL)))
		{
			char* p = (char*)pErrors->GetBufferPointer();
			MessageBoxA(0, p, 0, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);
		if (FAILED(Direct3D11::GetDevice()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, ppPixelShader)))
		{
			return E_FAIL;
		}
		return S_OK;
	}

	static HRESULT CreateConstantBuffer(ID3D11Buffer** ppConstantBuffer, UINT constantBufferSize)
	{
		//コンスタントバッファー作成　ここでは変換行列渡し用
		D3D11_BUFFER_DESC cb;
		cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cb.ByteWidth = constantBufferSize;
		cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cb.MiscFlags = 0;
		cb.StructureByteStride = 0;
		cb.Usage = D3D11_USAGE_DYNAMIC;

		if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, ppConstantBuffer)))
		{
			return E_FAIL;
		}
		return S_OK;
	}
}


// あとでメッシュなどのクラスに移動したいなー
/**
* @struct PrimitiveVertex
* @brief メッシュ
*/
struct PrimitiveVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;//法線　シェーディング（陰影計算）には法線は必須
};

struct PrimitiveVertex2D
{
	D3DXVECTOR3 pos;
};

/**
* @struct PrimitiveConstantBuffer
* @brief プリミティブの定数バッファー
*/
struct PrimitiveConstantBuffer
{
	D3DXMATRIX mW;//ワールド行列
	D3DXMATRIX mWVP;//ワールドから射影までの変換行列
	D3DXVECTOR4 lightDir;//ライト方向
	D3DXVECTOR4 color;//ディフューズ色	
	D3DXVECTOR4 eye;//視点	
};

/**
* @struct PrimitiveConstantBuffer2D
* @brief 2Dプリミティブの定数バッファー
*/
struct PrimitiveConstantBuffer2D
{
	alignas(16) D3DXMATRIX mW;
	alignas(16) D3DXVECTOR4 color;
	alignas(16) float viewPortWidth;
	alignas(16) float viewPortHeight;
};

/**
* @struct ShaderVariable
* @brief シェーダーの種類
*/
struct ShaderVariable
{
	ID3D11InputLayout* pInputLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11Buffer* pConstantBuffer;
	bool isAlive;
};

/**
* @enum SHADER_KIND
* @brief シェーダーの種類
*/
enum SHADER_KIND
{
	PRIMITIVE,
	PRIMITIVE2D,

	SHADER_KIND_MAX
};

/**
* @class ShaderDirector
* @brief シェーダーの管理クラス(シングルトン)
*/
class ShaderDirector
{
public:
	
	/**
	* @fn CreateInstance
	* @brief 自分のインスタンスを生成
	*/
	static void CreateInstance()
	{
		if (!m_this)
		{
			m_this = new ShaderDirector;
		}
	}

	/**
	* @fn DeleteInstance
	* @brief 自分のインスタンスを削除
	*/
	static void DeleteInstance()
	{
		if (m_this)
		{
			delete m_this;
			m_this = nullptr;
		}
	}
	 /**
	 * @fn GetShaderVariable
	 * @brief シェーダーで使う変数の取得
	 * @param[in] kind 使いたいシェーダーの種類
	 * @return ShaderVariable シェーダーで使う変数
	 */
	static const ShaderVariable GetShaderVariable(const SHADER_KIND kind)
	{
		for (int i = 0; i < SHADER_KIND_MAX; i++)
		{
			if (kind != i)
			{
				continue;
			}
			return m_this->m_shaderVariableArray[i];
		}
		return m_this->m_shaderVariableArray[0];
	}

	/**
	* @fn CreateShader
	* @brief シェーダーの作成
	* @param[in] kind 作りたいシェーダーの種類
	* @return HRESULT S_OKで成功
	*/
	static HRESULT CreateShader(const SHADER_KIND kind);

	/**
	* @fn ReleaseShader
	* @brief シェーダの削除
	* @param[in] kind 消したいシェーダーの種類
	*/
	static void ReleaseShader(const SHADER_KIND kind);

private:
	/**
	* @fn ShaderDirector
	* @brief コンストラクタ
	*/
	ShaderDirector();

	/**
	* @fn ~ShaderDirector
	* @brief デストラクタ
	*/
	~ShaderDirector();

	/**
	* @fn CreatePrimitiveShader
	* @brief プリミティブのシェーダーの作成
	* @return HRESULT S_OK
	*/
	HRESULT CreatePrimitiveShader();

	/**
	* @fn CreatePrimitive2DShader
	* @brief プリミティブ2Dのシェーダーの作成
	* @return HRESULT S_OK
	*/
	HRESULT CreatePrimitive2DShader();

	/**
	* @fn CreateVS
	* @brief バーテックスシェーダーの作成
	* @param[out] compiledShader XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	* @param[in] fileName シェーダーのファイル名
	* @param[in] kind シェーダーの種類
	* @return HRESULT 成功でS_OK
	*/
	HRESULT CreateVS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind);
	
	/**
	* @fn CreatePS
	* @brief ピクセルシェーダーの作成
	* @param[out] compiledShader XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	* @param[in] fileName シェーダーのファイル名
	* @param[in] kind シェーダーの種類
	* @return HRESULT 成功でS_OK
	*/
	HRESULT CreatePS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind);
	
	/**
	* @fn CreateCB
	* @brief コンスタントバッファーの作成
	* @param[in] constBufferSize コンスタントバッファーのサイズ(sizeof(STRUCT)みたいな感じ)
	* @param[in] kind シェーダーの種類
	* @return HRESULT 成功でS_OK
	*/
	HRESULT CreateCB(UINT constBufferSize, const SHADER_KIND kind);
	
	/**
	* @fn CreateIL
	* @brief InputLayoutの作成
	* @param[in] compiledShader XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	* @param[in] layoutArray layoutの配列
	* @param[in] elementCount 配列の要素数
	* @param[in] kind シェーダーの種類
	* @return HRESULT 成功でS_OK
	*/
	HRESULT CreateIL(ID3DBlob** compiledShader, D3D11_INPUT_ELEMENT_DESC* layoutArray, int elementCount, const SHADER_KIND kind);

	ShaderVariable m_shaderVariableArray[SHADER_KIND_MAX]; // シェーダーに使う変数の配列

	static ShaderDirector* m_this; // 静的な自分のポインタ
};

