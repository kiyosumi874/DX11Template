/**
* @file Point.cpp
* @brief 点を描画
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#include "Point.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"

/**
* @fn Point
* @brief コンストラクタ
*/
Point::Point()
{
	m_is2D = false;
	CreateVertexBuffer();
}

/**
* @fn Point
* @brief 引数付きコンストラクタ
* @param[in] pos 座標
* @param[in] is2D trueで2D描画
*/
Point::Point(const Vector3D& pos, bool is2D)
{
	m_is2D = is2D;
	m_pos = pos;
	CreateVertexBuffer();
}

/**
* @fn ~Point
* @brief デストラクタ
*/
Point::~Point()
{
	// 未実装
}

/**
* @fn Draw
* @brief 描画
*/
void Point::Draw()
{
	SetVertexBuffer();

	DrawCommon();
}

/**
* @fn CreateVertexBuffer
* @brief バーテックスバッファー作成
*/
void Point::CreateVertexBuffer()
{
	PrimitiveVertex vertices[] =
	{
		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
	};
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PrimitiveVertex) * 1;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
	{
		// エラーメッセージ
	}
}

/**
* @fn SetVertexBuffer
* @brief バーテックスバッファーをセット
*/
void Point::SetVertexBuffer()
{
	//バーテックスバッファーをセット
	UINT stride = sizeof(PrimitiveVertex);
	UINT offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
}
