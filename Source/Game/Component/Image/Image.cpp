/**
* @file Image.cpp
* @brief Imageの描画
* @author shiihara_kiyosumi
* @date 2022_08_11
*/

// ヘッダーファイルのインクルード
#include "Image.h"
#include "Game/Component/Object.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Common.h"
#include "Game/Component/Transform/Transform.h"

// using宣言
using D3D11::CreateVertexBuffer;
using D3D11::SetVertexBuffer;
using D3D11::CreateVertexShader;
using D3D11::CreateInputLayout;
using D3D11::CreatePixelShader;
using D3D11::CreateConstantBuffer;

void Image::Start()
{
}

void Image::Update()
{
}

void Image::Draw()
{
	// アルファブレンド用ブレンドステート作成
	if (m_isAlpha)
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState* pBlendState;
		Direct3D11::GetDevice()->CreateBlendState(&bd, &pBlendState);
		UINT mask = 0xffffffff;
		Direct3D11::GetDeviceContext()->OMSetBlendState(pBlendState, NULL, mask);
	}
	else
	{
		UINT mask = 0xffffffff;
		Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, mask);
	}
	

	SetVertexBuffer(&m_pVertexBuffer, sizeof(ImageVertex));

	//使用するシェーダーのセット
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);

	//シェーダーのコンスタントバッファーに各種データを渡す
	D3D11_MAPPED_SUBRESOURCE pData;
	ImageShaderConstBuffer cb;

	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
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
		D3DXMatrixTranspose(&world, &world);
		cb.world = world;
		cb.viewPortWidth = WINDOW_WIDTH;
		cb.viewPortHeight = WINDOW_HEIGHT;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer, 0);
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//頂点インプットレイアウトをセット
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pInputLayout);
	//プリミティブ・トポロジーをセット
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//テクスチャーをシェーダーに渡す
	Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &m_pSampler);
	Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);
	//プリミティブをレンダリング
	Direct3D11::GetDeviceContext()->Draw(4, 0);

}

void Image::Terminate()
{
	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pTexture);
}

/**
* @fn Init
* @brief 初期化
* @param[in] pos1 中心の位置
* @param[in] pos2 サイズ
* @param[in] texFileName テクスチャーのファイル名
* @param[in] isAlpha アルファブレンディングが有効か？trueで有効
*/
void Image::Init(const Vector3D& centerPos, const Vector3D& size, const char* texFileName, bool isAlpha, const int ID)
{
	m_isAlpha = isAlpha;
	m_ID = ID;
	m_parent->GetComponent<Transform>()->position = centerPos;
	/*auto posX = m_parent->GetComponent<Transform>()->position.x = pos1.x;
	auto posY = m_parent->GetComponent<Transform>()->position.y = pos1.y;*/
	//気をつけること。z値を１以上にしない。クリップ空間でz=1は最も奥を意味する。したがって描画されない。
	ImageVertex vertices[] =
	{
		D3DXVECTOR3(-size.x / 2,-size.y / 2, 0.0f), D3DXVECTOR2(0,0), // 頂点1
		D3DXVECTOR3( size.x / 2,-size.y / 2, 0.0f), D3DXVECTOR2(1,0), // 頂点2
		D3DXVECTOR3(-size.x / 2, size.y / 2, 0.0f), D3DXVECTOR2(0,1), // 頂点3
		D3DXVECTOR3( size.x / 2, size.y / 2, 0.0f), D3DXVECTOR2(1,1), // 頂点4
	};

	if (FAILED(CreateVertexBuffer(&m_pVertexBuffer, vertices, sizeof(vertices) / sizeof(vertices[0]))))
	{
		MyOutputDebugString("UIDirectorでCreateVertexBufferが失敗しました");
	}

	// ブロブ作成　ブロブとはシェーダーの塊みたいなもの。XXシェーダーとして特徴を持たない。後で各種シェーダーに成り得る。
	ID3DBlob* pCompiledShader = NULL;

	if (FAILED(CreateVertexShader(&m_pVertexShader, &pCompiledShader, "Shader/UI.hlsl", "VS")))
	{
		MyOutputDebugString("UIDirectorでCreateVertexShaderが失敗しました");
	}
	
	//インプットレイアウトを定義	
	// UIVertexがfloat3とfloat2を持っているためこうなる
	// シェーダーのほうは乗算などをするためにfloat4になっている
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(CreateInputLayout(&m_pInputLayout, &pCompiledShader, layout, numElements)))
	{
		MyOutputDebugString("UIDirectorでCreateInputLayoutが失敗しました");
		SAFE_RELEASE(pCompiledShader);
	}
	SAFE_RELEASE(pCompiledShader);

	if (FAILED(CreatePixelShader(&m_pPixelShader, &pCompiledShader, "Shader/UI.hlsl", "PS")))
	{
		MyOutputDebugString("UIDirectorでCreatePixelShaderが失敗しました");
	}
	SAFE_RELEASE(pCompiledShader);

	if (FAILED(CreateConstantBuffer(&m_pConstantBuffer, sizeof(ImageShaderConstBuffer))))
	{
		MyOutputDebugString("UIDirectorでCreateConstantBufferが失敗しました");
	}

	//テクスチャー用サンプラー作成
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampler);
	//テクスチャー読み込み
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(Direct3D11::GetDevice(), texFileName, NULL, NULL, &m_pTexture, NULL)))
	{
		MessageBoxA(0, "テクスチャーを読み込めません", "", MB_OK);
	}
}