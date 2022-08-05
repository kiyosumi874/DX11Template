#pragma once
#include "System/Math/Math.h"
#include "Game/Primitive/Primitive.h"

using math::Vector3D;


class Point : public Primitive
{
public:
	Point();
	Point(const Vector3D& pos, bool is2D = false);
	~Point() override;

	void Draw() override;

private:
	void CreateVertexBuffer();
	void SetVertexBuffer();

};
