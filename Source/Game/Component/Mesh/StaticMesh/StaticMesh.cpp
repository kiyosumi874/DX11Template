/**
* @file StaticMesh.h
* @brief StaticMesh�̕`��
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "StaticMesh.h"
#include "D3D9/Direct3D9.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Component/Object.h"
#include <d3dx11.h>
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>

// �K�v�ȃ��C�u�����t�@�C���̃��[�h
#pragma comment(lib,"d3dx11.lib")

/**
* @fn Start
* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
*/
void StaticMesh::Start()
{
}

void StaticMesh::Update()
{
	auto transform = m_parent->GetComponent<Transform>();
	transform->rotation.y += 0.01f;
}

void StaticMesh::Draw()
{
	UINT mask = 0xffffffff;
	Direct3D11::GetDeviceContext()->OMSetBlendState(NULL, NULL, mask);

	// using�錾
	using D3D11::SetVertexBuffer;
	SetVertexBuffer(&m_pVertexBuffer, m_pMesh->GetNumBytesPerVertex());

	//�g�p����V�F�[�_�[�̃Z�b�g
	Direct3D11::GetDeviceContext()->VSSetShader(m_pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(m_pPixelShader, NULL, 0);
	
	D3DXMATRIX world;

	// �A�t�B���ϊ�
	{
		auto transform = m_parent->GetComponent<Transform>();

		D3DXMATRIX scale;
		D3DXMATRIX rotate;
		D3DXMATRIX pos;

		D3DXMatrixIdentity(&world); // �s��̏�����

		D3DXMatrixScaling(&scale, transform->scale.x, transform->scale.y, transform->scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotate, transform->rotation.y, transform->rotation.x, transform->rotation.z);
		D3DXMatrixTranslation(&pos, transform->position.x, transform->position.y, transform->position.z);

		// DirectX�͍s�D��Ȃ̂�Scale�����Z
		D3DXMatrixMultiply(&world, &world, &scale);
		D3DXMatrixMultiply(&world, &world, &rotate);
		D3DXMatrixMultiply(&world, &world, &pos);
	}

	CameraData data;
	ZeroMemory(&data, sizeof(CameraData));
	bool isSuccess = TellCameraData::GetCameraData(&data, CAMERA_NUMBER::CAMERA_0);

	if (!isSuccess)
	{
		// �G���[���b�Z�[�W
		MyOutputDebugString("�J�����擾���s");
	}

	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		ConstantBuffer0 sg;
		//���[���h�s���n��
		sg.world = world;
		D3DXMatrixTranspose(&sg.world, &sg.world);
		//���[���h�A�J�����A�ˉe�s���n��
		sg.worldVewProj = world * data.matrixView * data.matrixProj;
		D3DXMatrixTranspose(&sg.worldVewProj, &sg.worldVewProj);
		//���C�g�̕�����n��
		D3DXVECTOR3 light(0, 0.5, -1);
		D3DXVec3Normalize(&light, &light);
		sg.lightDir = (D3DXVECTOR4)light;
		//���_�ʒu��n��
		sg.eye = D3DXVECTOR4(data.pos.x, data.pos.y, data.pos.z, 0);

		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer0));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer0, 0);
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@�|��`��
	for (DWORD i = 0; i < m_numAttribute; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (m_materialVec[m_attributeID[i]].numFace == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		Direct3D11::GetDeviceContext()->IASetIndexBuffer(m_pIndexBufferVec[i], DXGI_FORMAT_R32_UINT, 0);
		//Direct3D11::GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);
		//�}�e���A���̊e�v�f���G�t�F�N�g�i�V�F�[�_�[�j�ɓn��
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			ConstantBuffer1 sg;
			sg.ambient = m_materialVec[m_attributeID[i]].ambient;//�A���r�G���g�����V�F�[�_�[�ɓn��
			sg.diffuse = m_materialVec[m_attributeID[i]].diffuse;//�f�B�t���[�Y�J���[���V�F�[�_�[�ɓn��
			sg.specular = m_materialVec[m_attributeID[i]].specular;//�X�y�L�����[���V�F�[�_�[�ɓn��
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(ConstantBuffer1));
			Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer1, 0);
		}
		Direct3D11::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		Direct3D11::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (m_materialVec[m_attributeID[i]].pTexture)
		{
			Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &m_pSampler);
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &m_materialVec[m_attributeID[i]].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, Nothing);
		}
		//�v���~�e�B�u�������_�����O
		Direct3D11::GetDeviceContext()->DrawIndexed(m_materialVec[m_attributeID[i]].numFace * 3, 0, 0);
	}

}

void StaticMesh::Terminate()
{
	for (auto it : m_pIndexBufferVec)
	{
		SAFE_RELEASE(it);
	}
	m_pIndexBufferVec.clear();
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pVertexLayout);
	m_materialVec.clear();
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pD3DXMtrlBuffer);
	SAFE_RELEASE(m_pMesh);
}

/**
* @fn Init
* @brief ������
*/
HRESULT StaticMesh::Init(const char* fileName)
{
	if (FAILED(LoadXMesh(fileName)))
	{
		return E_FAIL;
	}

	// �}�e���A����pD3DXMtrlBuffer����擾
	if (FAILED(FetchMaterial()))
	{
		SAFE_RELEASE(m_pD3DXMtrlBuffer);
		return E_FAIL;
	}
	SAFE_RELEASE(m_pD3DXMtrlBuffer); // �����g���

	// �C���f�b�N�X�o�b�t�@�[�쐬
	if (FAILED(CreateIndexBuffer()))
	{
		return E_FAIL;
	}

	// �o�[�e�b�N�X�o�b�t�@�[�쐬
	if (FAILED(CreateVertexBuffer()))
	{
		return E_FAIL;
	}

	//�e�N�X�`���[�p�T���v���[�쐬
	if (FAILED(CreateSampler()))
	{
		return E_FAIL;
	}

	// using�錾
	using D3D11::CreateVertexShader;
	using D3D11::CreatePixelShader;
	using D3D11::CreateInputLayout;
	using D3D11::CreateConstantBuffer;

	//hlsl�t�@�C���ǂݍ��� �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3D10Blob* pCompiledShader = NULL;

	if (m_useTexture)
	{
		//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
		CreateVertexShader(&m_pVertexShader, &pCompiledShader, "Shader/Mesh.hlsl", "VS");
		//���_�C���v�b�g���C�A�E�g���`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = 3;
		CreateInputLayout(&m_pVertexLayout, &pCompiledShader, layout, numElements);
		//�u���u����s�N�Z���V�F�[�_�[�쐬
		CreatePixelShader(&m_pPixelShader, &pCompiledShader, "Shader/Mesh.hlsl", "PS");
	}
	else
	{
		//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
		CreateVertexShader(&m_pVertexShader, &pCompiledShader, "Shader/Mesh.hlsl", "VS_NoTex");
		//���_�C���v�b�g���C�A�E�g���`
		D3D11_INPUT_ELEMENT_DESC layout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		UINT numElements = 2;
		CreateInputLayout(&m_pVertexLayout, &pCompiledShader, layout, numElements);
		//�u���u����s�N�Z���V�F�[�_�[�쐬
		CreatePixelShader(&m_pPixelShader, &pCompiledShader, "Shader/Mesh.hlsl", "PS_NoTex");
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�ϊ��s��n���p
	CreateConstantBuffer(&m_pConstantBuffer0, sizeof(ConstantBuffer0));
	CreateConstantBuffer(&m_pConstantBuffer1, sizeof(ConstantBuffer1));

	return S_OK;
}

/**
* @fn LoadXMesh
* @brief xfile�����[�h����
* @param[in] fileName �t�@�C���̖��O
* @return HRESULT S_OK�Ő���
*/
HRESULT StaticMesh::LoadXMesh(const char* fileName)
{

	if (FAILED(D3DXLoadMeshFromXA(fileName, D3DXMESH_SYSTEMMEM | D3DXMESH_32BIT,
		Direct3D9::GetDevice(), NULL, &m_pD3DXMtrlBuffer, NULL,
		&m_numMaterial, &m_pMesh)))
	{
		MessageBoxA(NULL, "X�t�@�C���̓ǂݍ��݂Ɏ��s���܂���", NULL, MB_OK);
		return E_FAIL;
	}

	// ���̎��_�ŁA�t�@�C��������o�������b�V���f�[�^���ADx9��D3DX���b�V���ɓ����Ă���A
	// ���Ƃ́A��������D���ȏ������o����Dx11�̎��O���b�V���ɗ��p���邾���B

	

	return S_OK;
}

HRESULT StaticMesh::FetchMaterial()
{
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)m_pD3DXMtrlBuffer->GetBufferPointer();
	m_materialVec.resize(m_numMaterial);
	for (DWORD i = 0; i < m_numMaterial; i++)
	{
		//�A���r�G���g
		m_materialVec[i].ambient.x = d3dxMaterials[i].MatD3D.Ambient.r;
		m_materialVec[i].ambient.y = d3dxMaterials[i].MatD3D.Ambient.g;
		m_materialVec[i].ambient.z = d3dxMaterials[i].MatD3D.Ambient.b;
		m_materialVec[i].ambient.w = d3dxMaterials[i].MatD3D.Ambient.a;
		//�f�B�t���[�Y
		m_materialVec[i].diffuse.x = d3dxMaterials[i].MatD3D.Diffuse.r;
		m_materialVec[i].diffuse.y = d3dxMaterials[i].MatD3D.Diffuse.g;
		m_materialVec[i].diffuse.z = d3dxMaterials[i].MatD3D.Diffuse.b;
		m_materialVec[i].diffuse.w = d3dxMaterials[i].MatD3D.Diffuse.a;
		//�X�y�L�����[
		m_materialVec[i].specular.x = d3dxMaterials[i].MatD3D.Specular.r;
		m_materialVec[i].specular.y = d3dxMaterials[i].MatD3D.Specular.g;
		m_materialVec[i].specular.z = d3dxMaterials[i].MatD3D.Specular.b;
		m_materialVec[i].specular.w = d3dxMaterials[i].MatD3D.Specular.a;
		//�e�N�X�`���[�������
		if (d3dxMaterials[i].pTextureFilename != NULL &&
			lstrlenA(d3dxMaterials[i].pTextureFilename) > 0)
		{
			m_useTexture = true;
			strcpy_s(m_materialVec[i].textureName, d3dxMaterials[i].pTextureFilename);
			//�e�N�X�`���[���쐬
			if (FAILED(D3DX11CreateShaderResourceViewFromFileA(Direct3D11::GetDevice(),
				m_materialVec[i].textureName, NULL, NULL, &m_materialVec[i].pTexture, NULL)))
			{
				return E_FAIL;
			}
		}
	}
	return S_OK;
}

HRESULT StaticMesh::CreateIndexBuffer()
{
	//�C���f�b�N�X�o�b�t�@�[���쐬
	m_pIndexBufferVec.resize(m_numMaterial);
	//���b�V���̑������𓾂�B�������ŃC���f�b�N�X�o�b�t�@�[����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���
	D3DXATTRIBUTERANGE* pAttrTable = NULL;

	m_pMesh->OptimizeInplace(D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 0, 0, 0, 0);
	m_pMesh->GetAttributeTable(NULL, &m_numAttribute);
	pAttrTable = new D3DXATTRIBUTERANGE[m_numAttribute];
	if (FAILED(m_pMesh->GetAttributeTable(pAttrTable, &m_numAttribute)))
	{
		MessageBoxA(0, "�����e�[�u���擾���s", "", MB_OK);
		return E_FAIL;
	}
	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�C���f�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B
	int* pIndex = NULL;
	m_pMesh->LockIndexBuffer(D3DLOCK_READONLY, (void**)&pIndex);

	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;

	//�������Ƃ̃C���f�b�N�X�o�b�t�@���쐬
	for (DWORD i = 0; i < m_numAttribute; i++)
	{
		m_attributeID[i] = pAttrTable[i].AttribId;
		//Dx9��D3DMESH�̃C���f�b�N�X�o�b�t�@�[����̏��ŁADx11�̃C���f�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(int) * pAttrTable[i].FaceCount * 3;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		InitData.pSysMem = &pIndex[pAttrTable[i].FaceStart * 3];//�傫���C���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(*3��Y�ꂸ�Ɂj

		if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pIndexBufferVec[i])))
		{
			return E_FAIL;
		}
		m_materialVec[m_attributeID[i]].numFace = pAttrTable[i].FaceCount;
	}
	delete[] pAttrTable;
	m_pMesh->UnlockIndexBuffer();
	return S_OK;
}

HRESULT StaticMesh::CreateVertexBuffer()
{
	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	D3D11_SUBRESOURCE_DATA InitData;
	//D3DXMESH�̏ꍇ�A���b�N���Ȃ���D3DX�o�[�e�b�N�X�o�b�t�@�[������o���Ȃ��̂Ń��b�N����B
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	m_pMesh->GetVertexBuffer(&pVB);
	DWORD dwStride = m_pMesh->GetNumBytesPerVertex();
	BYTE* pVertices = NULL;
	MyVertex* pvVertex = NULL;
	if (SUCCEEDED(pVB->Lock(0, 0, (VOID**)&pVertices, 0)))
	{
		pvVertex = (MyVertex*)pVertices;
		//Dx9��D3DMESH�̃o�[�e�b�N�X�o�b�t�@�[����̏��ŁADx11�̃o�[�e�b�N�X�o�b�t�@���쐬
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = m_pMesh->GetNumBytesPerVertex() * m_pMesh->GetNumVertices();
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		InitData.pSysMem = pvVertex;

		//�e�N�X�`���[���W���}�C�i�X�΍�
		if (m_useTexture)
		{
			for (int i = 0; i < m_pMesh->GetNumVertices(); i++)
			{
				if (pvVertex[i].tex.x < 0)
				{
					pvVertex[i].tex.x += 1;
				}
				if (pvVertex[i].tex.y < 0)
				{
					pvVertex[i].tex.y += 1;
				}
			}
		}

		if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
			return E_FAIL;

		pVB->Unlock();
	}
	SAFE_RELEASE(pVB);
	return S_OK;
}

HRESULT StaticMesh::CreateSampler()
{
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if (FAILED(Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampler)))
	{
		return E_FAIL;
	}

	return S_OK;
}
