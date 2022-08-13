/**
* @file Image.h
* @brief Imageの描画
* @author shiihara_kiyosumi
* @date 2022_08_11
*/

// ヘッダーファイルのインクルード
#pragma once
#include "Game/Component/Component.h"
#include "Game/Component/Transform/Transform.h"
#include "System/Math/Math.h"
#include <D3D11.h>
#include <D3DX10.h>

// using宣言
using math::Vector3D;

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
class Image : public Component
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

private:
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
