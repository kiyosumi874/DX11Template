/**
* @file Direct3D11.h
* @brief Direct3D11周りを整理する
* @author shiihara_kiyosumi
* @date 2022_07_29
*/

// ヘッダーファイルのインクルード
#pragma once
#include <Windows.h>
#include <D3D11.h>
	
/**
* @class Direct3D11
* @brief Direct3D11周りを整理するクラス(シングルトン)
*/
class Direct3D11
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
			m_this = new Direct3D11;
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
	* @fn Init
	* @brief 初期化
	* @param[in] hWnd ウィンドウハンドル(カレントウィンドウに割り当てられたハンドル)
	* @return HRESULT S_OKで成功
	*/
	static HRESULT Init(HWND hWnd);

	/**
	* @fn Clear
	* @brief 単色で画面を塗りつぶす
	*/
	static void Clear(int red = 255, int green = 255, int blue = 255);

	/**
	* @fn Present
	* @brief 画面更新
	*/
	static HRESULT Present();

	/**
	* @fn GetDevice
	* @brief D3D11Deviceの取得
	*/
	static ID3D11Device* GetDevice() { return m_this->m_pDevice; }

	/**
	* @fn GetDeviceContext
	* @brief D3D11DeviceContextの取得
	*/
	static ID3D11DeviceContext* GetDeviceContext() { return m_this->m_pDeviceContext; }

private:
	/**
	* @fn Direct3D11
	* @brief コンストラクタ
	*/
	Direct3D11();

	/**
	* @fn Direct3D11
	* @brief デストラクタ
	*/
	~Direct3D11();

	ID3D11Device* m_pDevice; // <ビデオカード>という物理的デバイスをモデル化したもの
	ID3D11DeviceContext* m_pDeviceContext; // GraphicsDeviceInterface(GDI)が管理するデータ構造であり出力デバイス(ディスプレイなど)の属性が格納される
	IDXGISwapChain* m_pSwapChain; // <画面>をモデル化したオブジェクト(フロントバッファと一個以上のバックバッファのこと)

	// ここにあるビューは「リソースとレンダリングパイプラインを関連付けるオブジェクト」のこと
	// 単なるバイトの塊であるリソースを使うにはビューが必要になる
	// ビューはC言語における型キャストのようなもの
	ID3D11RenderTargetView* m_pRenderTargetView; // レンダーターゲットとリソースをつなげる橋のようなもの
	ID3D11DepthStencilView* m_pDepthStencilView; // デプスステンシルとリソースをつなげる橋のようなもの
	
	// DepthBufferはピクセル毎の深度情報を記録しておくもので、これにより物体の前後関係を正しく表示させる事が出来る
	// StencilBufferはオンタイムでマスクの変更が自由にできる
	ID3D11Texture2D* m_pDepthStencilBuffer; // 二次元的なバッファーなのでTexture2D.名前に惑わされるな

	ID3D11DepthStencilState* m_pDepthStencilState; // depthStencilの設定
	ID3D11BlendState* m_pBlendState; // ブレンドの設定
	ID3D11RasterizerState* m_pRasterizerState; // ラスタライザーの設定

	static Direct3D11* m_this; // 静的な自分のポインタ
};