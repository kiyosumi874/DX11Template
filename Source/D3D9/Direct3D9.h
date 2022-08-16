/**
* @file Direct3D9.h
* @brief Direct3D9周りを整理する
* @author shiihara_kiyosumi
* @date 2022_08_14
*/

// ヘッダーファイルのインクルード
#pragma once
#include <d3dx9.h>

// 必要なライブラリファイルのロード
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"d3d9.lib")

/**
* @class Direct3D9
* @brief Direct3D9周りを整理するクラス(シングルトン)
*/
class Direct3D9
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
			m_this = new Direct3D9;
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

	static HRESULT Init();

	/**
	* @fn GetDevice
	* @brief D3D9Deviceの取得
	*/
	static LPDIRECT3DDEVICE9 GetDevice() { return m_this->m_pDevice9; }

private:
	/**
	* @fn Direct3D9
	* @brief コンストラクタ
	*/
	Direct3D9();

	/**
	* @fn ~Direct3D9
	* @brief デストラクタ
	*/
	~Direct3D9();

	LPDIRECT3D9 m_pD3d9; // D3D9のオブジェクト
	LPDIRECT3DDEVICE9 m_pDevice9; // D3D9のデバイス

	static Direct3D9* m_this; // 静的な自分のポインタ
};