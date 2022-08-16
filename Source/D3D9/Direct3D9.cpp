#include "Direct3D9.h"
#include "System/Common.h"
#include "System/Window/Window.h"

Direct3D9* Direct3D9::m_this = nullptr;


HRESULT Direct3D9::Init()
{
	// 「Direct3D」オブジェクトの作成
	m_this->m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == m_this->m_pD3d9)
	{
		MessageBoxA(0, "Direct3D9の作成に失敗しました", "", MB_OK);
		return E_FAIL;
	}
	// 「DIRECT3Dデバイス」オブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	auto hWnd = Window::GetWindowHandle();

	if (FAILED(m_this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_this->m_pDevice9)))
	{
		if (FAILED(m_this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_this->m_pDevice9)))
		{
			MessageBoxA(0, "HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します", NULL, MB_OK);
			if (FAILED(m_this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_this->m_pDevice9)))
			{
				if (FAILED(m_this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_this->m_pDevice9)))
				{
					MessageBoxA(0, "DIRECT3Dデバイスの作成に失敗しました", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}

Direct3D9::Direct3D9()
{
	// クラスが初期化される
	ZeroMemory(this, sizeof(Direct3D9));
}

Direct3D9::~Direct3D9()
{
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d9);
}
