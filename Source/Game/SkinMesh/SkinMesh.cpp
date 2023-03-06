/**
* @file SkinMesh.cpp
* @brief SkinMesh�̕`��(�܂����S�ł͂Ȃ�)
* @author shiihara_kiyosumi
* @date 2022_08_18
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "SkinMesh.h"
#include "D3D11/Direct3D11.h"
#include "D3D9/Direct3D9.h"
#include "Game/Component/Object.h"
#include "System/Transform.h"
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>

#pragma warning(disable:4996)

/**
* @fn SkinMesh
* @breif �R���X�g���N�^
*/
SkinMesh::SkinMesh()
{
}

/**
* @fn ~SkinMesh
* @breif �f�X�g���N�^
*/
SkinMesh::~SkinMesh()
{
	delete[] m_BoneArray;
	delete[] m_pMaterial;
	SAFE_RELEASE(m_pSampler);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pConstantBufferLight);
	SAFE_RELEASE(m_pConstantBufferWVPMaterial);
	SAFE_RELEASE(m_pConstantBufferBone);
	delete[] m_ppIndexBuffer;
	SAFE_DELETE(m_pMeshParser);
}

/**
* @fn Draw
* @breif �`��
* @param[in] transform �`��ʒu
*/
void SkinMesh::Draw(const Transform& transform)
{
	const auto& devCon11 = Direct3D11::GetDeviceContext();

	//�g�p����V�F�[�_�[�̃Z�b�g
	devCon11->VSSetShader(m_pVertexShader, NULL, 0);
	devCon11->PSSetShader(m_pPixelShader, NULL, 0);

	D3DXMATRIX world;

	// �A�t�B���ϊ�
	{
		D3DXMATRIX scale;
		D3DXMATRIX rotate;
		D3DXMATRIX pos;

		D3DXMatrixIdentity(&world); // �s��̏�����

		D3DXMatrixScaling(&scale, transform.GetScale().x, transform.GetScale().y, transform.GetScale().z);
		D3DXMatrixRotationYawPitchRoll(&rotate, transform.GetRotate().y, transform.GetRotate().x, transform.GetRotate().z);
		D3DXMatrixTranslation(&pos, transform.GetPos().x, transform.GetPos().y, transform.GetPos().z);

		// DirectX�͍s�D��Ȃ̂�Scale�����Z
		D3DXMatrixMultiply(&world, &world, &scale);
		D3DXMatrixMultiply(&world, &world, &rotate);
		D3DXMatrixMultiply(&world, &world, &pos);
	}



	// �{�[���̏����V�F�[�_�[�ɓn��
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devCon11->Map(m_pConstantBufferBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_GLOBAL_BONES sg;
			for (int i = 0; i < m_iNumBone; i++)
			{
				D3DXMATRIX mat = GetCurrentPoseMatrix(i);
				D3DXMatrixTranspose(&mat, &mat);
				sg.mBone[i] = mat;
			}
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_GLOBAL_BONES));
			devCon11->Unmap(m_pConstantBufferBone, 0);
		}
		devCon11->VSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
		devCon11->PSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
	}


	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�i�o�[�e�b�N�X�o�b�t�@�[�͈�ł����j
	UINT stride = sizeof(MY_SKINVERTEX);
	UINT offset = 0;
	devCon11->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	devCon11->IASetInputLayout(m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	devCon11->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CameraData cam;
	ZeroMemory(&cam, sizeof(CameraData));
	bool isSuccess = TellCameraData::GetCameraData(&cam, CAMERA_NUMBER::CAMERA_0);

	if (!isSuccess)
	{
		// �G���[���b�Z�[�W
		MyOutputDebugString("�J�����擾���s");
	}

	//�J�����ʒu���V�F�[�_�[�ɓn��
	{
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBufferLight, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_SKIN_GLOBAL0 sg;
			sg.vLightDir = D3DXVECTOR4(1, 1, -1, 0.0f);
			sg.vEye = D3DXVECTOR4(cam.pos.x, cam.pos.y, cam.pos.z, 0);
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL0));
			Direct3D11::GetDeviceContext()->Unmap(m_pConstantBufferLight, 0);
		}
		devCon11->VSSetConstantBuffers(0, 1, &m_pConstantBufferLight);
		devCon11->PSSetConstantBuffers(0, 1, &m_pConstantBufferLight);
	}


	//�}�e���A���̐������A���ꂼ��̃}�e���A���̃C���f�b�N�X�o�b�t�@�|��`��
	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//�g�p����Ă��Ȃ��}�e���A���΍�
		if (m_pMaterial[i].dwNumFace == 0)
		{
			continue;
		}
		//�C���f�b�N�X�o�b�t�@�[���Z�b�g
		stride = sizeof(int);
		offset = 0;
		devCon11->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//�}�e���A���̊e�v�f�ƕϊ��s����V�F�[�_�[�ɓn��			
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(devCon11->Map(m_pConstantBufferWVPMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
		{
			SHADER_SKIN_GLOBAL1 sg;
			sg.mW = world;
			D3DXMatrixTranspose(&sg.mW, &sg.mW);
			sg.mWVP = world * cam.matrixView * cam.matrixProj;
			D3DXMatrixTranspose(&sg.mWVP, &sg.mWVP);
			sg.vAmbient = m_pMaterial[i].Ka;
			sg.vDiffuse = m_pMaterial[i].Kd;
			sg.vSpecular = m_pMaterial[i].Ks;
			memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL1));
			devCon11->Unmap(m_pConstantBufferWVPMaterial, 0);
		}
		devCon11->VSSetConstantBuffers(1, 1, &m_pConstantBufferWVPMaterial);
		devCon11->PSSetConstantBuffers(1, 1, &m_pConstantBufferWVPMaterial);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			devCon11->PSSetSamplers(0, 1, &m_pSampler);
			devCon11->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			devCon11->PSSetShaderResources(0, 1, Nothing);
		}
		//Draw
		devCon11->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}

	//�A�j���[�V�����t���[����i�߂�@�X�L�����X�V
	SetNewPoseMatrices();

	//�A�j���i�s
	if (m_pMeshParser->m_pAnimController)
	{
		m_pMeshParser->m_pAnimController->AdvanceTime(0.01666, NULL);
	}
	D3DXMATRIX m /*= world * cam.matrixView * cam.matrixProj*/;
	D3DXMatrixIdentity(&m);
	m_pMeshParser->UpdateFrameMatrices(m_pMeshParser->m_pFrameRoot, &m);
}

/**
* @fn Init
* @breif ������
* @param[in] fileName ���f���̃t�@�C����
* @return HRESULT S_OK�Ő���
*/
HRESULT SkinMesh::Init(LPCSTR fileName)
{
	auto hr = CreateFromX(fileName);
	//D3D11�֘A�̏�����
	ID3DBlob* pCompiledShader = NULL;
	ID3DBlob* pErrors = NULL;
	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile("Shader/Geometry_Material_Texture_Skin.hlsl", NULL, NULL, "VSSkin", "vs_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		int size = pErrors->GetBufferSize();
		char* ch = (char*)pErrors->GetBufferPointer();
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);

	if (FAILED(Direct3D11::GetDevice()->CreateVertexShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�o�[�e�b�N�X�V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);

	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layout, numElements, pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), &m_pVertexLayout)))
		return E_FAIL;
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//�u���u����s�N�Z���V�F�[�_�[�쐬
	if (FAILED(D3DX11CompileFromFile("Shader/Geometry_Material_Texture_Skin.hlsl", NULL, NULL, "PSSkin", "ps_5_0", 0, 0, NULL, &pCompiledShader, &pErrors, NULL)))
	{
		MessageBox(0, "hlsl�ǂݍ��ݎ��s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreatePixelShader(pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pPixelShader)))
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox(0, "�s�N�Z���V�F�[�_�[�쐬���s", NULL, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	D3D11_BUFFER_DESC cb;
	//�R���X�^���g�o�b�t�@�[0�쐬
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL0);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferLight)))
	{
		return E_FAIL;
	}
	//�R���X�^���g�o�b�t�@�[1�쐬  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_SKIN_GLOBAL1);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferWVPMaterial)))
	{
		return E_FAIL;
	}
	//�R���X�^���g�o�b�t�@�[�{�[���p�@�쐬  
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof(SHADER_GLOBAL_BONES);
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBufferBone)))
	{
		return E_FAIL;
	}
	//�e�N�X�`���[�p�T���v���[�쐬
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampler);

	return S_OK;
}

/**
* @fn ReadSkinInfo
* @brief xfile����X�L���֘A�̏���ǂݏo��
* @param[in] pvVB ���_�f�[�^�̍\���̂̃|�C���^
* @return HRESULT S_OK�Ő���
*/
HRESULT SkinMesh::ReadSkinInfo(MY_SKINVERTEX* pvVB)
{
	//X���璊�o���ׂ����́A���_���Ƃ̃{�[���C���f�b�N�X�A���_���Ƃ̃{�[���E�F�C�g�A�o�C���h�s��A�|�[�Y�s��@��4����

	int i, k, m, n;
	int iNumVertex = m_pMeshParser->GetNumVertices();//���_��
	int iNumBone = 0;//�{�[����


	//�{�[��	�̌��𓾂�
	iNumBone = m_pMeshParser->GetNumBones();
	//���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
	for (i = 0; i < iNumBone; i++)
	{
		int iNumIndex = m_pMeshParser->GetNumBoneVertices(i);//���̃{�[���ɉe�����󂯂钸�_��
		int* piIndex = new int[iNumIndex];
		for (k = 0; k < iNumIndex; k++) piIndex[k] = m_pMeshParser->GetBoneVerticesIndices(i, k);
		double* pdWeight = new double[iNumIndex];
		for (k = 0; k < iNumIndex; k++) pdWeight[k] = m_pMeshParser->GetBoneVerticesWeights(i, k);
		//���_������C���f�b�N�X�����ǂ��āA���_�T�C�h�Ő�������
		for (k = 0; k < iNumIndex; k++)
		{
			//X��CG�\�t�g���{�[��4�{�ȓ��Ƃ͌���Ȃ��B5�{�ȏ�̏ꍇ�́A�d�݂̑傫������4�{�ɍi�� 

			//�E�F�C�g�̑傫�����Ƀ\�[�g�i�o�u���\�[�g�j
			for (m = 4; m > 1; m--)
			{
				for (n = 1; n < m; n++)
				{
					if (pvVB[piIndex[k]].bBoneWeight[n - 1] < pvVB[piIndex[k]].bBoneWeight[n])
					{
						float tmp = pvVB[piIndex[k]].bBoneWeight[n - 1];
						pvVB[piIndex[k]].bBoneWeight[n - 1] = pvVB[piIndex[k]].bBoneWeight[n];
						pvVB[piIndex[k]].bBoneWeight[n] = tmp;
						int itmp = pvVB[piIndex[k]].bBoneIndex[n - 1];
						pvVB[piIndex[k]].bBoneIndex[n - 1] = pvVB[piIndex[k]].bBoneIndex[n];
						pvVB[piIndex[k]].bBoneIndex[n] = itmp;
					}
				}
			}
			//�\�[�g��́A�Ō�̗v�f�Ɉ�ԏ������E�F�C�g�������Ă���͂��B
			bool flag = false;
			for (m = 0; m < 4; m++)
			{
				if (pvVB[piIndex[k]].bBoneWeight[m] == 0)
				{
					flag = true;
					pvVB[piIndex[k]].bBoneIndex[m] = i;
					pvVB[piIndex[k]].bBoneWeight[m] = pdWeight[k];
					break;
				}
			}
			if (flag == false)
			{
				pvVB[piIndex[k]].bBoneIndex[3] = i;
				pvVB[piIndex[k]].bBoneWeight[3] = pdWeight[k];
				break;
			}

		}
		delete piIndex;
		delete pdWeight;
	}
	//
	//�{�[���𐶐�
	m_iNumBone = iNumBone;
	m_BoneArray = new BONE[iNumBone];
	//�|�[�Y�s��𓾂� �����|�[�Y
	for (i = 0; i < m_iNumBone; i++)
	{
		m_BoneArray[i].mBindPose = m_pMeshParser->GetBindPose(i);
	}

	return S_OK;
}

/**
* @fn CreateFromX
* @brief xfile����X�L�����b�V�����쐬����
* @param[in] szFileName ���f���̃t�@�C����
* @return HRESULT S_OK�Ő���
*/
HRESULT SkinMesh::CreateFromX(LPCSTR szFileName)
{
	//X�t�@�C���ǂݍ���
	m_pMeshParser = new D3DXPARSER;
	m_pMeshParser->LoadMeshFromX(Direct3D9::GetDevice(), szFileName);

	//���O�ɒ��_���A�|���S�������𒲂ׂ�
	DWORD dwNumVert = m_pMeshParser->GetNumVertices();
	DWORD dwNumFace = m_pMeshParser->GetNumFaces();
	DWORD dwNumUV = m_pMeshParser->GetNumUVs();
	if (dwNumVert < dwNumUV)//Direct3D�ł�UV�̐��������_���K�v
	{
		//���L���_���ŁA���_��������Ȃ���
		MessageBox(0, "Direct3D�́AUV�̐��������_���K�v�ł��iUV��u���ꏊ���K�v�ł��j�e�N�X�`���[�͐������\���Ȃ��Ǝv���܂�", NULL, MB_OK);
		return E_FAIL;
	}
	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[dwNumVert];
	int* piFaceBuffer = new int[dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

	//���_�ǂݍ���
	for (int i = 0; i < dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pMeshParser->GetVertexCoord(i);
		pvVB[i].vPos.x = v.x;
		pvVB[i].vPos.y = v.y;
		pvVB[i].vPos.z = v.z;
	}
	//�|���S�����i���_�C���f�b�N�X�j�ǂݍ���
	for (int i = 0; i < dwNumFace * 3; i++)
	{
		piFaceBuffer[i] = m_pMeshParser->GetIndex(i);
	}
	//�@���ǂݍ���
	for (int i = 0; i < dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pMeshParser->GetNormal(i);
		pvVB[i].vNorm.x = v.x;
		pvVB[i].vNorm.y = v.y;
		pvVB[i].vNorm.z = v.z;
	}
	//�e�N�X�`���[���W�ǂݍ���
	for (int i = 0; i < dwNumVert; i++)
	{
		D3DXVECTOR2 v = m_pMeshParser->GetUV(i);
		pvVB[i].vTex.x = v.x;
		pvVB[i].vTex.y = v.y;
	}
	//�}�e���A���ǂݍ���
	m_dwNumMaterial = m_pMeshParser->GetNumMaterials();
	m_pMaterial = new MY_SKINMATERIAL[m_dwNumMaterial];

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	m_ppIndexBuffer = new ID3D11Buffer*[m_dwNumMaterial];
	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//����	
		m_pMaterial[i].Ka.x = m_pMeshParser->GetAmbient(i).y;
		m_pMaterial[i].Ka.y = m_pMeshParser->GetAmbient(i).z;
		m_pMaterial[i].Ka.z = m_pMeshParser->GetAmbient(i).w;
		m_pMaterial[i].Ka.w = m_pMeshParser->GetAmbient(i).x;
		//�g�U���ˌ�	
		m_pMaterial[i].Kd.x = m_pMeshParser->GetDiffuse(i).y;
		m_pMaterial[i].Kd.y = m_pMeshParser->GetDiffuse(i).z;
		m_pMaterial[i].Kd.z = m_pMeshParser->GetDiffuse(i).w;
		m_pMaterial[i].Kd.w = m_pMeshParser->GetDiffuse(i).x;
		//���ʔ��ˌ�
		m_pMaterial[i].Ks.x = m_pMeshParser->GetSpecular(i).y;
		m_pMaterial[i].Ks.y = m_pMeshParser->GetSpecular(i).z;
		m_pMaterial[i].Ks.z = m_pMeshParser->GetSpecular(i).w;
		m_pMaterial[i].Ks.w = m_pMeshParser->GetSpecular(i).x;

		//�e�N�X�`���[�i�f�B�t���[�Y�e�N�X�`���[�̂݁j
		char* name = m_pMeshParser->GetTexturePath(i);
		if (name)
		{
			strcpy(m_pMaterial[i].szTextureName, name);
		}
		//�e�N�X�`���[���쐬
		if (m_pMaterial[i].szTextureName[0] != 0 && FAILED(D3DX11CreateShaderResourceViewFromFileA(Direct3D11::GetDevice(), m_pMaterial[i].szTextureName, NULL, NULL, &m_pMaterial[i].pTexture, NULL)))//��΃p�X�t�@�C�����́A�܂����s����Ǝv�����A�A�A
		{
			MessageBox(0, "�e�N�X�`���[�ǂݍ��ݎ��s", NULL, MB_OK);
			return E_FAIL;
		}

		//���̃}�e���A���ł���C���f�b�N�X�z����̊J�n�C���f�b�N�X�𒲂ׂ�@����ɃC���f�b�N�X�̌������ׂ�
		int iCount = 0;
		int* pIndex = new int[dwNumFace * 3];//�Ƃ肠�����A���b�V�����̃|���S�����Ń������m�ہi�X�̃|���S���O���[�v�͂��Ȃ炸����ȉ��ɂȂ邪�j

		for (int k = 0; k < dwNumFace; k++)
		{
			if (i == m_pMeshParser->GeFaceMaterialIndex(k))//���� i == k�Ԗڂ̃|���S���̃}�e���A���ԍ� �Ȃ�
			{
				pIndex[iCount] = m_pMeshParser->GetFaceVertexIndex(k, 0);//k�Ԗڂ̃|���S������钸�_�̃C���f�b�N�X�@1��
				pIndex[iCount + 1] = m_pMeshParser->GetFaceVertexIndex(k, 1);//2��
				pIndex[iCount + 2] = m_pMeshParser->GetFaceVertexIndex(k, 2);//3��
				iCount += 3;
			}
		}
		if (iCount > 0) CreateIndexBuffer(iCount * sizeof(int), pIndex, &m_ppIndexBuffer[i]);
		delete pIndex;
		m_pMaterial[i].dwNumFace = iCount / 3;//���̃}�e���A�����̃|���S����

	}
	//�X�L�����i�W���C���g�A�E�F�C�g�@�j�ǂݍ���
	ReadSkinInfo(pvVB);
	//�o�[�e�b�N�X�o�b�t�@�[���쐬
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(MY_SKINVERTEX) * dwNumVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return E_FAIL;

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete piFaceBuffer;
	if (pvVB) delete pvVB;

	return S_OK;
}

/**
* @fn CreateIndexBuffer
* @brief �C���f�b�N�X�o�b�t�@�[�쐬
* @param[in] dwSize �o�b�t�@�[�T�C�Y
* @param[in] pIndex �������f�[�^�ւ̃|�C���^�[
* @param[out, optional] ppIndexBuffer �쐬���ꂽ�o�b�t�@�[�I�u�W�F�N�g��ID3D11Buffer�C���^�[�t�F�C�X�ւ̃|�C���^�[�̃A�h���X
* @return HRESULT S_OK�Ő���
*/
HRESULT SkinMesh::CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer)
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dwSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, ppIndexBuffer)))
	{
		return E_FAIL;
	}

	return S_OK;
}

/**
* @fn SetNewPoseMatrices
* @brief �{�[�������̃|�[�Y�ʒu�ɃZ�b�g����
* @return HRESULT S_OK�Ő���
*/
void SkinMesh::SetNewPoseMatrices()
{
	//�]�ރt���[����Update���邱�ƁB���Ȃ��ƍs�񂪍X�V����Ȃ�
	//m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot)�������_�����O���Ɏ��s���邱��

	//�܂��A�A�j���[�V�������ԂɌ��������s����X�V����̂�D3DXMESH�ł�
	//�A�j���[�V�����R���g���[���[���i���Łj����Ă������̂Ȃ̂ŁA�A�j���[�V�����R���g���[���[
	//���g���ăA�j����i�s�����邱�Ƃ��K�v
	//	m_pD3dxMesh->m_pAnimController->AdvanceTime(....)�������_�����O���Ɏ��s���邱��

	for (int i = 0; i < m_iNumBone; i++)
	{
		m_BoneArray[i].mNewPose = m_pMeshParser->GetNewPose(i);
	}
}

/**
* @fn GetCurrentPoseMatrix
* @brief ����(���݂�)�|�[�Y�s���Ԃ�
* @param[in] index �{�[���z��̓Y����
* @return D3DXMATRIX ����(���݂�)�|�[�Y�s��
*/
D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
{
	D3DXMATRIX ret = m_BoneArray[index].mBindPose * m_BoneArray[index].mNewPose;
	return ret;
}

/**
* @fn GetBindPoseMatrix
* @brief �o�C���h�|�[�Y�s���Ԃ�
* @param[in] index �{�[���z��̓Y����
* @return D3DXMATRIX �o�C���h�|�[�Y�s��
*/
D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
{
	return m_BoneArray[index].mBindPose;
}

/**
* @fn GetBoneNames
* @brief �{�[���̖��O�擾
* @param[in] iBoneIndex �{�[���z��̓Y����
* @return CHAR* �{�[���̖��O
*/
CHAR* SkinMesh::GetBoneNames(int iBoneIndex)
{
	return m_pMeshParser->GetBoneName(iBoneIndex);
}
