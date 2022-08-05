#include "Point.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"


Point::Point()
{
	m_is2D = false;
	CreateVertexBuffer();
}

Point::Point(const Vector3D& pos, bool is2D)
{
	m_is2D = is2D;
	m_pos = pos;
	CreateVertexBuffer();
}

Point::~Point()
{
}


void Point::Draw()
{
	SetVertexBuffer();

	DrawCommon();
}

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

void Point::SetVertexBuffer()
{
	//バーテックスバッファーをセット
	UINT stride = sizeof(PrimitiveVertex);
	UINT offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
}
