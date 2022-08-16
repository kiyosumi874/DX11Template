#include "Direct3D9.h"
#include "System/Common.h"
#include "System/Window/Window.h"

Direct3D9* Direct3D9::m_this = nullptr;


HRESULT Direct3D9::Init()
{
	// �uDirect3D�v�I�u�W�F�N�g�̍쐬
	m_this->m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == m_this->m_pD3d9)
	{
		MessageBoxA(0, "Direct3D9�̍쐬�Ɏ��s���܂���", "", MB_OK);
		return E_FAIL;
	}
	// �uDIRECT3D�f�o�C�X�v�I�u�W�F�N�g�̍쐬
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
			MessageBoxA(0, "HAL���[�h��DIRECT3D�f�o�C�X���쐬�ł��܂���\nREF���[�h�ōĎ��s���܂�", NULL, MB_OK);
			if (FAILED(m_this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_this->m_pDevice9)))
			{
				if (FAILED(m_this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_this->m_pDevice9)))
				{
					MessageBoxA(0, "DIRECT3D�f�o�C�X�̍쐬�Ɏ��s���܂���", NULL, MB_OK);
					return E_FAIL;
				}
			}
		}
	}
	return S_OK;
}

Direct3D9::Direct3D9()
{
	// �N���X�������������
	ZeroMemory(this, sizeof(Direct3D9));
}

Direct3D9::~Direct3D9()
{
	SAFE_RELEASE(m_pDevice9);
	SAFE_RELEASE(m_pD3d9);
}
