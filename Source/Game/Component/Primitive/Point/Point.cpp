/**
* @file Point.cpp
* @brief 点を描画(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#include "Point.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Component/Transform/Transform.h"
#include "Game/Component/Object.h"

/**
* @fn Start
* @brief 生成したときに最初に一回だけ走る関数
*/
void Point::Start()
{
	m_transform = m_parent->GetComponent<Transform>();
}

/**
* @fn Draw
* @brief 描画
*/
void Point::Draw()
{
	SetVertexBuffer();
	
	DrawCommon();
	//プリミティブ・トポロジーをセット
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//プリミティブをレンダリング
	Direct3D11::GetDeviceContext()->Draw(1, 0);
}

/**
* @fn Init
* @brief 初期化(絶対呼び出して)
* @param[in] pos1 描画する点の座標
*/
void Point::Init(const Vector3D& pos1)
{
	CreateVertexBuffer(pos1);
}

/**
* @fn CreateVertexBuffer
* @brief バーテックスバッファー作成
* @param[in] pos1 描画する点の座標
*/
void Point::CreateVertexBuffer(const Vector3D& pos1)
{
	PrimitiveVertex vertices[] =
	{
		D3DXVECTOR3(pos1.x, pos1.y, pos1.z)
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
