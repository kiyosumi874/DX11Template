/**
* @file Direct3D11.cpp
* @brief Direct3D11周りを整理する
* @author shiihara_kiyosumi
* @date 2022_08_02
*/

// ヘッダーファイルのインクルード
#include "Direct3D11.h"
#include "System/Common.h"

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3d11.lib")

Direct3D11* Direct3D11::m_this = nullptr;

/**
* @fn Direct3D11
* @brief コンストラクタ
*/
Direct3D11::Direct3D11()
{
	// クラスが初期化される
	ZeroMemory(this, sizeof(Direct3D11));
}

/**
* @fn ~Direct3D11
* @brief デストラクタ
*/
Direct3D11::~Direct3D11()
{
	SAFE_RELEASE(m_pRasterizerState);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	SAFE_RELEASE(m_pDepthStencilState);
	SAFE_RELEASE(m_pBlendState);
	SAFE_RELEASE(m_pDeviceContext);
	SAFE_RELEASE(m_pDevice);
}

/**
* @fn Init
* @brief 初期化
* @param[in] hWnd ウィンドウハンドル(カレントウィンドウに割り当てられたハンドル)
* @return HRESULT S_OKで成功
*/
HRESULT Direct3D11::Init(HWND hWnd)
{
	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// フィーチャーレベルの設定
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = NULL;

	// デバイスとスワップチェーンの作成
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		0, &pFeatureLevels, 1, D3D11_SDK_VERSION, &sd, &m_this->m_pSwapChain, &m_this->m_pDevice,
		pFeatureLevel, &m_this->m_pDeviceContext)))
	{
		return E_FAIL;
	}

	// バックバッファーテクスチャーを取得(既にあるので作成ではない)
	ID3D11Texture2D* pBackBuffer;
	m_this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	m_this->m_pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_this->m_pRenderTargetView);
	SAFE_RELEASE(pBackBuffer);

	// デプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC descDepth;
	descDepth.Width = WINDOW_WIDTH;
	descDepth.Height = WINDOW_HEIGHT;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	m_this->m_pDevice->CreateTexture2D(&descDepth, NULL, &m_this->m_pDepthStencilBuffer);
	// そのテクスチャーに対しデプスステンシルビュー(DSV)を作成
	m_this->m_pDevice->CreateDepthStencilView(m_this->m_pDepthStencilBuffer, NULL, &m_this->m_pDepthStencilView);
	// レンダーターゲットビューとデプスステンシルビューをパイプラインにセット
	m_this->m_pDeviceContext->OMSetRenderTargets(1, &m_this->m_pRenderTargetView, m_this->m_pDepthStencilView);

	// 深度ステンシルステートを作成
	D3D11_DEPTH_STENCIL_DESC dc;
	ZeroMemory(&dc, sizeof(dc));
	dc.DepthEnable = true;
	dc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dc.DepthFunc = D3D11_COMPARISON_LESS;
	dc.StencilEnable = false;
	if (FAILED(m_this->m_pDevice->CreateDepthStencilState(&dc, &m_this->m_pDepthStencilState)))
	{
		return E_FAIL;
	}
	// 深度ステンシルステートを適用
	m_this->m_pDeviceContext->OMSetDepthStencilState(m_this->m_pDepthStencilState, 0);

	// ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = WINDOW_WIDTH;
	vp.Height = WINDOW_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_this->m_pDeviceContext->RSSetViewports(1, &vp);

	// ラスタライズ設定
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.CullMode = D3D11_CULL_NONE;
	rdc.FillMode = D3D11_FILL_SOLID;
	rdc.FrontCounterClockwise = TRUE;
	m_this->m_pDevice->CreateRasterizerState(&rdc, &m_this->m_pRasterizerState);
	m_this->m_pDeviceContext->RSSetState(m_this->m_pRasterizerState);

	// アルファブレンド用ブレンドステート作成
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
	if (FAILED(m_this->m_pDevice->CreateBlendState(&bd, &m_this->m_pBlendState)))
	{
		return E_FAIL;
	}
	UINT mask = 0xffffffff;
	m_this->m_pDeviceContext->OMSetBlendState(m_this->m_pBlendState, NULL, mask);

	return S_OK;
}

/**
* @fn Clear
* @brief 単色で画面を塗りつぶす
*/
void Direct3D11::Clear(int red, int green, int blue)
{
	min(red, 255);
	min(green, 255);
	min(blue, 255);
	//画面クリア（実際は単色で画面を塗りつぶす処理）
	float ClearColor[4] = { static_cast<float>(red) / 255.0f,static_cast<float>(green) / 255.0f,static_cast<float>(blue) / 255.0f,1.0f }; // クリア色作成　RGBAの順
	m_this->m_pDeviceContext->ClearRenderTargetView(m_this->m_pRenderTargetView, ClearColor); // 画面クリア
	m_this->m_pDeviceContext->ClearDepthStencilView(m_this->m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0); // 深度バッファクリア
}

/**
* @fn Present
* @brief 画面更新
*/
HRESULT Direct3D11::Present()
{
	m_this->m_pSwapChain->Present(0, 0); //画面更新(バックバッファをフロントバッファに)
	return S_OK;
}
