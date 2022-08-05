#pragma once
#include <D3D11.h>
#include "System/Math/Math.h"

using math::Vector3D;

class Primitive
{
public:
	Primitive();
	virtual ~Primitive();

	virtual void Draw() = 0;

	Vector3D m_pos;
protected:
	void DrawCommon();
	ID3D11Buffer* m_pVertexBuffer;
	bool m_is2D;

};
