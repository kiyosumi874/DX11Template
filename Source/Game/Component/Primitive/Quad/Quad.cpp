/**
* @file Quad.cpp
* @brief 四角形を描画(コンポーネント)
* @author shiihara_kiyosumi
* @date 2022_08_10
*/

// ヘッダーファイルのインクルード
#include "Quad.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Component/Transform/Transform.h"
#include "Game/Component/Object.h"
#include <cassert>

/**
* @fn Start
* @brief 生成したときに最初に一回だけ走る関数
*/
void Quad::Start()
{
	m_transform = m_parent->GetComponent<Transform>();
}

/**
* @fn Draw
* @brief 描画
*/
void Quad::Draw()
{
	SetVertexBuffer();

	DrawCommon();
	//プリミティブ・トポロジーをセット
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//プリミティブをレンダリング
	Direct3D11::GetDeviceContext()->Draw(4, 0);
}

/**
* @fn Init
* @brief 初期化(絶対呼び出して)
* @param[in] pos1 描画する四角形の座標
* @param[in] pos2 描画する四角形の座標
* @param[in] pos3 描画する四角形の座標
* @param[in] pos3 描画する四角形の座標
*/
void Quad::Init(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, const Vector3D& pos4)
{
	CreateVertexBuffer(pos1, pos2, pos3, pos4);
}

/**
* @fn SetVertexBuffer
* @brief バーテックスバッファーをセット
* @param[in] pos1 描画する四角形の座標
* @param[in] pos2 描画する四角形の座標
* @param[in] pos3 描画する四角形の座標
* @param[in] pos3 描画する四角形の座標
*/
void Quad::CreateVertexBuffer(const Vector3D& pos1, const Vector3D& pos2, const Vector3D& pos3, const Vector3D& pos4)
{
	if (m_is2D)
	{
		PrimitiveVertex2D vertices[] =
		{
			D3DXVECTOR3(pos1.x, pos1.y, 0.0f),
			D3DXVECTOR3(pos2.x, pos2.y, 0.0f),
			D3DXVECTOR3(pos3.x, pos3.y, 0.0f),
			D3DXVECTOR3(pos4.x, pos4.y, 0.0f),
		};
		assert(SUCCEEDED(D3D11::CreateVertexBuffer<PrimitiveVertex2D>(&m_pVertexBuffer, vertices, 4)));
	}
	else
	{
		PrimitiveVertex vertices[] =
		{
			D3DXVECTOR3(pos1.x, pos1.y, pos1.z),D3DXVECTOR3(0,0,-1),
			D3DXVECTOR3(pos2.x, pos2.y, pos2.z),D3DXVECTOR3(0,0,-1),
			D3DXVECTOR3(pos3.x, pos3.y, pos3.z),D3DXVECTOR3(0,0,-1),
			D3DXVECTOR3(pos4.x, pos4.y, pos4.z),D3DXVECTOR3(0,0,-1),
		};
		assert(SUCCEEDED(D3D11::CreateVertexBuffer<PrimitiveVertex>(&m_pVertexBuffer, vertices, 4)));
	}
	
}

/**
* @fn SetVertexBuffer
* @brief バーテックスバッファーをセット
*/
void Quad::SetVertexBuffer()
{
	//バーテックスバッファーをセット
	if (m_is2D)
	{
		UINT stride = sizeof(PrimitiveVertex2D);
		UINT offset = 0;
		Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}
	else
	{
		UINT stride = sizeof(PrimitiveVertex);
		UINT offset = 0;
		Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	}
}
