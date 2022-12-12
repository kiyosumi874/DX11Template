/**
* @file Image.h
* @brief Imageの描画
* @author shiihara_kiyosumi
* @date 2022_08_11
*/

// ヘッダーファイルのインクルード
#pragma once
#include "System/Transform.h"
#include <D3D11.h>
#include <D3DX10.h>


/**
* @struct ImageShaderConstBuffer
* @brief ImageShader用のコンストバッファー,シェーダー側と同じでなければならない
*/
struct ImageShaderConstBuffer
{
	alignas(16) D3DXMATRIX world;
	alignas(16) float viewPortWidth;
	alignas(16) float viewPortHeight;
};

/**
* @struct ImageVertex
* @brief 頂点の構造体
*/
struct ImageVertex
{
	D3DXVECTOR3 pos; // 位置
	D3DXVECTOR2 uv;  // テクスチャー座標
};


/**
* @class Image
* @brief Imageクラス
*/
class Image
{
public:
	/**
	* @fn コンストラクタ
	* @brief 生成したときに最初に一回だけ走る関数
	*/
	Image();

	/**
	* @fn デストラクタ
	* @brief 終了処理
	*/
	~Image();

	/**
	* @fn Update
	* @brief 更新
	*/
	void Update();

	/**
	* @fn Draw
	* @brief 描画
	*/
	void Draw();

	

	/**
	* @fn Init
	* @brief 初期化
	* @param[in] pos1 中心の位置
	* @param[in] pos2 サイズ
	* @param[in] texFileName テクスチャーのファイル名
	* @param[in] isAlpha アルファブレンディングが有効か？trueで有効
	*/
	void Init(const Vector3D& centerPos, const Vector3D& size, const char* texFileName, bool isAlpha, const int ID = -1);

	const int GetID() const
	{
		return m_ID;
	}

	// getter
	const Vector3D& GetPos() const
	{
		return m_transform->GetPos();
	}

	const Vector3D& GetRotate() const
	{
		return m_transform->GetRotate();
	}

	const Vector3D& GetScale() const
	{
		return m_transform->GetScale();
	}

	// setter
	void SetPos(const Vector3D& pos)
	{
		m_transform->SetPos(pos);
	}

	void SetRotate(const Vector3D& rotate)
	{
		m_transform->SetPos(rotate);
	}

	void SetScale(const Vector3D& scale)
	{
		m_transform->SetPos(scale);
	}

private:
	Transform* m_transform;
	bool m_isAlpha; // アルファブレンディングが有効か？trueで有効
	int m_ID;
	ID3D11InputLayout* m_pInputLayout; // インプットレイアウト
	ID3D11VertexShader* m_pVertexShader; // バーテックスシェーダー
	ID3D11PixelShader* m_pPixelShader; // ピクセルシェーダー
	ID3D11Buffer* m_pConstantBuffer; // 定数バッファー
	ID3D11Buffer* m_pVertexBuffer; // バーテックスバッファー

	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
};
