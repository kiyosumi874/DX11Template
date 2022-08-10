/**
* @file ShaderDirector.h
* @brief シェーダーの管理
* @details ShaderDirectorクラスはシングルトンなので注意
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#include "ShaderDirector.h"
#include "System/Common.h"
#include "D3D11/Direct3D11.h"
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <cassert>

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dCompiler.lib")

// クラスの静的変数の初期化
ShaderDirector* ShaderDirector::m_this = nullptr;

/**
* @fn CreateShader
* @brief シェーダーの作成
* @param[in] kind 作りたいシェーダーの種類
* @return HRESULT S_OKで成功
*/
HRESULT ShaderDirector::CreateShader(const SHADER_KIND kind)
{
	assert(!m_this->m_shaderVariableArray[kind].isAlive);
	switch (kind)
	{
	case PRIMITIVE:
		m_this->CreatePrimitiveShader();
		break;
	case PRIMITIVE2D:
		m_this->CreatePrimitive2DShader();
		break;
	default:
		return E_FAIL;
		break;
	}

	return S_OK;
}

/**
* @fn ReleaseShader
* @brief シェーダの削除
* @param[in] kind 消したいシェーダーの種類
*/
void ShaderDirector::ReleaseShader(const SHADER_KIND kind)
{
	assert(m_this->m_shaderVariableArray[kind].isAlive);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pConstantBuffer);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pPixelShader);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pInputLayout);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pVertexShader);
	m_this->m_shaderVariableArray[kind].isAlive = false;
}

/**
* @fn ShaderDirector
* @brief コンストラクタ
*/
ShaderDirector::ShaderDirector()
{
	for (int i = 0; i < SHADER_KIND_MAX; i++)
	{
		m_shaderVariableArray[i].pConstantBuffer = NULL;
		m_shaderVariableArray[i].pPixelShader = NULL;
		m_shaderVariableArray[i].pInputLayout = NULL;
		m_shaderVariableArray[i].pVertexShader = NULL;
		m_shaderVariableArray[i].isAlive = false;
	}
}

/**
* @fn ~ShaderDirector
* @brief デストラクタ
*/
ShaderDirector::~ShaderDirector()
{
	for (int i = 0; i < SHADER_KIND_MAX; i++)
	{
		SAFE_RELEASE(m_shaderVariableArray[i].pConstantBuffer);
		SAFE_RELEASE(m_shaderVariableArray[i].pPixelShader);
		SAFE_RELEASE(m_shaderVariableArray[i].pInputLayout);
		SAFE_RELEASE(m_shaderVariableArray[i].pVertexShader);
		m_shaderVariableArray[i].isAlive = false;
	}
}

/**
* @fn CreatePrimitiveShader
* @brief プリミティブのシェーダーの作成
* @return HRESULT S_OK
*/
HRESULT ShaderDirector::CreatePrimitiveShader()
{
	// ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob* pCompiledShader = NULL;

	// バーテックスシェーダー作成
	m_this->CreateVS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitiveVS.hlsl", PRIMITIVE);

	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_this->CreateIL(&pCompiledShader, layout, 1, PRIMITIVE);
	SAFE_RELEASE(pCompiledShader);

	// ピクセルシェーダー作成
	m_this->CreatePS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitivePS.hlsl", PRIMITIVE);
	SAFE_RELEASE(pCompiledShader);

	//コンスタントバッファー作成　ここでは変換行列渡し用
	m_this->CreateCB(sizeof(PrimitiveConstantBuffer), PRIMITIVE);
	
	return S_OK;
}

/**
* @fn CreatePrimitive2DShader
* @brief プリミティブ2Dのシェーダーの作成
* @return HRESULT S_OK
*/
HRESULT ShaderDirector::CreatePrimitive2DShader()
{
	// ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob* pCompiledShader = NULL;

	// バーテックスシェーダー作成
	if (FAILED(m_this->CreateVS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitiveVS2D.hlsl", PRIMITIVE2D)))
	{
		return E_FAIL;
	}

	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_this->CreateIL(&pCompiledShader, layout, 1, PRIMITIVE2D);

	SAFE_RELEASE(pCompiledShader);

	// ピクセルシェーダー作成
	if (FAILED(m_this->CreatePS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitivePS2D.hlsl", PRIMITIVE2D)))
	{
		SAFE_RELEASE(pCompiledShader);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//コンスタントバッファー作成　ここでは変換行列渡し用
	m_this->CreateCB(sizeof(PrimitiveConstantBuffer2D), PRIMITIVE2D);
	
	return S_OK;
}

/**
* @fn CreateVS
* @brief バーテックスシェーダーの作成
* @param[out] compiledShader XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
* @param[in] fileName シェーダーのファイル名
* @param[in] kind シェーダーの種類
* @return HRESULT 成功でS_OK
*/
HRESULT ShaderDirector::CreateVS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind)
{
	auto& comShader = *compiledShader;
	ID3DBlob* pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &comShader, &pErrors, NULL)))
	{
		char* p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreateVertexShader(comShader->GetBufferPointer(), comShader->GetBufferSize(), NULL, (ID3D11VertexShader**)&m_this->m_shaderVariableArray[kind].pVertexShader)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
* @fn CreatePS
* @brief ピクセルシェーダーの作成
* @param[out] compiledShader XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
* @param[in] fileName シェーダーのファイル名
* @param[in] kind シェーダーの種類
* @return HRESULT 成功でS_OK
*/
HRESULT ShaderDirector::CreatePS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind)
{
	auto& comShader = *compiledShader;
	ID3DBlob* pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &comShader, &pErrors, NULL)))
	{
		char* p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreatePixelShader(comShader->GetBufferPointer(), comShader->GetBufferSize(), NULL, (ID3D11PixelShader**)&m_this->m_shaderVariableArray[kind].pPixelShader)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
* @fn CreateCB
* @brief コンスタントバッファーの作成
* @param[in] constBufferSize コンスタントバッファーのサイズ(sizeof(STRUCT)みたいな感じ)
* @param[in] kind シェーダーの種類
* @return HRESULT 成功でS_OK
*/
HRESULT ShaderDirector::CreateCB(UINT constBufferSize, const SHADER_KIND kind)
{
	//コンスタントバッファー作成　ここでは変換行列渡し用
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = constBufferSize;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_this->m_shaderVariableArray[kind].pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
* @fn CreateIL
* @brief InputLayoutの作成
* @param[in] compiledShader XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
* @param[in] layoutArray layoutの配列
* @param[in] elementCount 配列の要素数
* @param[in] kind シェーダーの種類
* @return HRESULT 成功でS_OK
*/
HRESULT ShaderDirector::CreateIL(ID3DBlob** compiledShader, D3D11_INPUT_ELEMENT_DESC* layoutArray, int elementCount, const SHADER_KIND kind)
{
	auto& comShader = *compiledShader;
	//頂点インプットレイアウトを作成
	if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layoutArray, elementCount, comShader->GetBufferPointer(), comShader->GetBufferSize(), &m_this->m_shaderVariableArray[kind].pInputLayout)))
	{
		return E_FAIL;
	}
	return S_OK;
}
