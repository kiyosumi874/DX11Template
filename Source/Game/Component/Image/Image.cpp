/**
* @file Image.cpp
* @brief Image�̕`��
* @author shiihara_kiyosumi
* @date 2022_08_11
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Image.h"
#include "Game/Component/Object.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "System/Common.h"

// using�錾
using D3D11::CreateVertexBuffer;
using D3D11::SetVertexBuffer;
using D3D11::CreateVertexShader;
using D3D11::CreateInputLayout;
using D3D11::CreatePixelShader;
using D3D11::CreateConstantBuffer;


Image::Image()
{
	m_transform = new Transform();
}

Image::~Image()
{
	// ��܃N���X
	SAFE_DELETE(m_transform);

	SAFE_RELEASE(m_pInputLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pConstantBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pTexture);
}

void Image::Update()
{
}

void Image::Draw()
{
	// �A���t�@�u�����h�p�u�����h�X�e�[�g�쐬
	if (m_isAlpha)
	{
		D3D11_BLEND_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
		bd.IndependentBlendEnable = false;
		bd.AlphaToCoverageEnable = false;
		bd.RenderTarget[0].BlendEnable = true;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState* pBlendState;
		Direct3D11::GetDevice()->CreateBlendState(&bd, &pBlendState);
		UINT mask = 0xffffffff;
		Direct3D11::GetDeviceContext()->OMSetBlendState(pBlendState, NULL, mask);
	}
	else
	{
		UINT mask = 0xffffffff;
		Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, mask);
	}
	

	SetVertexBuffer(&m_pVertexBuffer, sizeof(ImageVertex));

	//�g�p����V�F�[�_�[�̃Z�b�g
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	ImageShaderConstBuffer cb;

	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX world;

		// �A�t�B���ϊ�
		{

			D3DXMATRIX scale;
			D3DXMATRIX rotate;
			D3DXMATRIX pos;

			D3DXMatrixIdentity(&world); // �s��̏�����

			D3DXMatrixScaling(&scale, m_transform->GetScale().x, m_transform->GetScale().y, m_transform->GetScale().z);
			D3DXMatrixRotationYawPitchRoll(&rotate, m_transform->GetRotate().y, m_transform->GetRotate().x, m_transform->GetRotate().z);
			D3DXMatrixTranslation(&pos, m_transform->GetPos().x, m_transform->GetPos().y, m_transform->GetPos().z);

			// DirectX�͍s�D��Ȃ̂�Scale�����Z
			D3DXMatrixMultiply(&world, &world, &scale);
			D3DXMatrixMultiply(&world, &world, &rotate);
			D3DXMatrixMultiply(&world, &world, &pos);
		}
		D3DXMatrixTranspose(&world, &world);
		cb.world = world;
		cb.viewPortWidth = WINDOW_WIDTH;
		cb.viewPortHeight = WINDOW_HEIGHT;
		memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pInputLayout);
	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//�e�N�X�`���[���V�F�[�_�[�ɓn��
	Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &m_pSampler);
	Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pTexture);
	//�v���~�e�B�u�������_�����O
	Direct3D11::GetDeviceContext()->Draw(4, 0);

}


/**
* @fn Init
* @brief ������
* @param[in] pos1 ���S�̈ʒu
* @param[in] pos2 �T�C�Y
* @param[in] texFileName �e�N�X�`���[�̃t�@�C����
* @param[in] isAlpha �A���t�@�u�����f�B���O���L�����Htrue�ŗL��
*/
void Image::Init(const Vector3D& centerPos, const Vector3D& size, const char* texFileName, bool isAlpha, const int ID)
{
	m_isAlpha = isAlpha;
	m_ID = ID;
	m_transform->SetPos(centerPos);
	/*auto posX = m_parent->GetComponent<Transform>()->position.x = pos1.x;
	auto posY = m_parent->GetComponent<Transform>()->position.y = pos1.y;*/
	//�C�����邱�ƁBz�l���P�ȏ�ɂ��Ȃ��B�N���b�v��Ԃ�z=1�͍ł������Ӗ�����B���������ĕ`�悳��Ȃ��B
	ImageVertex vertices[] =
	{
		D3DXVECTOR3(-size.x / 2,-size.y / 2, 0.0f), D3DXVECTOR2(0,0), // ���_1
		D3DXVECTOR3( size.x / 2,-size.y / 2, 0.0f), D3DXVECTOR2(1,0), // ���_2
		D3DXVECTOR3(-size.x / 2, size.y / 2, 0.0f), D3DXVECTOR2(0,1), // ���_3
		D3DXVECTOR3( size.x / 2, size.y / 2, 0.0f), D3DXVECTOR2(1,1), // ���_4
	};

	if (FAILED(CreateVertexBuffer(&m_pVertexBuffer, vertices, sizeof(vertices) / sizeof(vertices[0]))))
	{
		MyOutputDebugString("UIDirector��CreateVertexBuffer�����s���܂���");
	}

	// �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob* pCompiledShader = NULL;

	if (FAILED(CreateVertexShader(&m_pVertexShader, &pCompiledShader, "Shader/UI.hlsl", "VS")))
	{
		MyOutputDebugString("UIDirector��CreateVertexShader�����s���܂���");
	}
	
	//�C���v�b�g���C�A�E�g���`	
	// UIVertex��float3��float2�������Ă��邽�߂����Ȃ�
	// �V�F�[�_�[�̂ق��͏�Z�Ȃǂ����邽�߂�float4�ɂȂ��Ă���
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	if (FAILED(CreateInputLayout(&m_pInputLayout, &pCompiledShader, layout, numElements)))
	{
		MyOutputDebugString("UIDirector��CreateInputLayout�����s���܂���");
		SAFE_RELEASE(pCompiledShader);
	}
	SAFE_RELEASE(pCompiledShader);

	if (FAILED(CreatePixelShader(&m_pPixelShader, &pCompiledShader, "Shader/UI.hlsl", "PS")))
	{
		MyOutputDebugString("UIDirector��CreatePixelShader�����s���܂���");
	}
	SAFE_RELEASE(pCompiledShader);

	if (FAILED(CreateConstantBuffer(&m_pConstantBuffer, sizeof(ImageShaderConstBuffer))))
	{
		MyOutputDebugString("UIDirector��CreateConstantBuffer�����s���܂���");
	}

	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampler);
	//�e�N�X�`���[�ǂݍ���
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(Direct3D11::GetDevice(), texFileName, NULL, NULL, &m_pTexture, NULL)))
	{
		MessageBoxA(0, "�e�N�X�`���[��ǂݍ��߂܂���", "", MB_OK);
	}
}