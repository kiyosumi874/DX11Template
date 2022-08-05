#pragma once
#include <d3dx10.h>

enum class CAMERA_NUMBER
{
	CAMERA_0
};

struct CameraData
{
	D3DXMATRIX matrixView;
	D3DXMATRIX matrixProj;
	CAMERA_NUMBER key;
};