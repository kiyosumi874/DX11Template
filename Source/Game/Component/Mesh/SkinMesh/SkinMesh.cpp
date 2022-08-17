#include "SkinMesh.h"
#include "D3D11/Direct3D11.h"
#include "D3D9/Direct3D9.h"
#include "Game/Component/Object.h"
#include "Game/Component/Transform/Transform.h"
#include <Game/Camera/CameraStruct.h>
#include <Game/Camera/TellCameraData.h>

#pragma warning(disable:4996)


void SkinMesh::Start()
{
}

void SkinMesh::Terminate()
{
	delete[] m_BoneArray;
	delete[] m_pMaterial;
	SAFE_RELEASE(m_pSampleLinear);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pConstantBuffer0);
	SAFE_RELEASE(m_pConstantBuffer1);
	SAFE_RELEASE(m_pConstantBufferBone);
	/*for (int i = 0; i < m_dwNumMaterial; i++)
	{
		SAFE_RELEASE(m_ppIndexBuffer[i]);
	}*/
	delete[] m_ppIndexBuffer;
	SAFE_DELETE(m_pD3dxMesh);
}

void SkinMesh::Update()
{
}

void SkinMesh::Draw()
{
	D3D11_MAPPED_SUBRESOURCE pData;

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

	CameraData cam;
	ZeroMemory(&cam, sizeof(CameraData));
	bool isSuccess = TellCameraData::GetCameraData(&cam, CAMERA_NUMBER::CAMERA_0);

	if (!isSuccess)
	{
		// �G���[���b�Z�[�W
		MyOutputDebugString("�J�����擾���s");
	}

	//�A�j���[�V�����t���[����i�߂�@�X�L�����X�V
	static int iFrame = 0;

	if (++iFrame >= 3600) iFrame = 0;
	SetNewPoseMatrices(iFrame);

	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBufferBone, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_GLOBAL_BONES sg;
		for (int i = 0; i < m_iNumBone; i++)
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix(i);
			D3DXMatrixTranspose(&mat, &mat);
			sg.mBone[i] = mat;
		}
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_GLOBAL_BONES));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBufferBone, 0);
	}
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(2, 1, &m_pConstantBufferBone);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(2, 1, &m_pConstantBufferBone);

	//�o�[�e�b�N�X�o�b�t�@�[���Z�b�g�i�o�[�e�b�N�X�o�b�t�@�[�͈�ł����j
	UINT stride = sizeof(MY_SKINVERTEX);
	UINT offset = 0;
	Direct3D11::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D11::GetDeviceContext()->IASetInputLayout(m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g
	Direct3D11::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//�J�����ʒu���V�F�[�_�[�ɓn��
	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer0, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		SHADER_SKIN_GLOBAL0 sg;
		sg.vLightDir = D3DXVECTOR4(1, 1, -1, 0.0f);
		sg.vEye = D3DXVECTOR4(cam.pos.x, cam.pos.y, cam.pos.z, 0);
		memcpy_s(pData.pData, pData.RowPitch, (void*)&sg, sizeof(SHADER_SKIN_GLOBAL0));
		Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer0, 0);
	}
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &m_pConstantBuffer0);

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
		Direct3D11::GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[i], DXGI_FORMAT_R32_UINT, 0);

		//�}�e���A���̊e�v�f�ƕϊ��s����V�F�[�_�[�ɓn��			
		D3D11_MAPPED_SUBRESOURCE pData;
		if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(m_pConstantBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
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
			Direct3D11::GetDeviceContext()->Unmap(m_pConstantBuffer1, 0);
		}
		Direct3D11::GetDeviceContext()->VSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		Direct3D11::GetDeviceContext()->PSSetConstantBuffers(1, 1, &m_pConstantBuffer1);
		//�e�N�X�`���[���V�F�[�_�[�ɓn��
		if (m_pMaterial[i].szTextureName[0] != NULL)
		{
			Direct3D11::GetDeviceContext()->PSSetSamplers(0, 1, &m_pSampleLinear);
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, &m_pMaterial[i].pTexture);
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			Direct3D11::GetDeviceContext()->PSSetShaderResources(0, 1, Nothing);
		}
		//Draw
		Direct3D11::GetDeviceContext()->DrawIndexed(m_pMaterial[i].dwNumFace * 3, 0, 0);
	}
	//�A�j���i�s
	if (m_pD3dxMesh->m_pAnimController)
	{
		m_pD3dxMesh->m_pAnimController->AdvanceTime(0.01666, NULL);
	}
	D3DXMATRIX m /*= world * cam.matrixView * cam.matrixProj*/;
	D3DXMatrixIdentity(&m);
	m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot, &m);
}

//������
HRESULT SkinMesh::Init(LPCSTR fileName)
{
	CreateFromX(fileName);
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
		return FALSE;
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

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBuffer0)))
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

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_pConstantBuffer1)))
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
	Direct3D11::GetDevice()->CreateSamplerState(&SamDesc, &m_pSampleLinear);

	return S_OK;
}
//
//HRESULT CD3DXSKINMESH::ReadSkinInfo(KFbxMesh* pFbxMesh,MY_SKINVERTEX* pvVB)
//X����X�L���֘A�̏���ǂݏo���@
HRESULT SkinMesh::ReadSkinInfo(MY_SKINVERTEX* pvVB)
{
	//X���璊�o���ׂ����́A���_���Ƃ̃{�[���C���f�b�N�X�A���_���Ƃ̃{�[���E�F�C�g�A�o�C���h�s��A�|�[�Y�s��@��4����

	int i, k, m, n;
	int iNumVertex = m_pD3dxMesh->GetNumVertices();//���_��
	int iNumBone = 0;//�{�[����


	//�{�[��	�̌��𓾂�
	iNumBone = m_pD3dxMesh->GetNumBones();
	//���ꂼ��̃{�[���ɉe�����󂯂钸�_�𒲂ׂ�@��������t�ɁA���_�x�[�X�Ń{�[���C���f�b�N�X�E�d�݂𐮓ڂ���
	for (i = 0; i < iNumBone; i++)
	{
		int iNumIndex = m_pD3dxMesh->GetNumBoneVertices(i);//���̃{�[���ɉe�����󂯂钸�_��
		int* piIndex = new int[iNumIndex];
		for (k = 0; k < iNumIndex; k++) piIndex[k] = m_pD3dxMesh->GetBoneVerticesIndices(i, k);
		double* pdWeight = new double[iNumIndex];
		for (k = 0; k < iNumIndex; k++) pdWeight[k] = m_pD3dxMesh->GetBoneVerticesWeights(i, k);
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
		m_BoneArray[i].mBindPose = m_pD3dxMesh->GetBindPose(i);
	}

	return S_OK;
}
//
//HRESULT CD3DXSKINMESH::CreateFromX(CHAR* szFileName)
//X����X�L�����b�V�����쐬����@�@���Ӂj�f�ށiX)�̂ق��́A�O�p�|���S���ɂ��邱��
HRESULT SkinMesh::CreateFromX(LPCSTR szFileName)
{
	//X�t�@�C���ǂݍ���
	m_pD3dxMesh = new D3DXPARSER;
	m_pD3dxMesh->LoadMeshFromX(Direct3D9::GetDevice(), szFileName);

	//���O�ɒ��_���A�|���S�������𒲂ׂ�
	m_dwNumVert = m_pD3dxMesh->GetNumVertices();
	m_dwNumFace = m_pD3dxMesh->GetNumFaces();
	m_dwNumUV = m_pD3dxMesh->GetNumUVs();
	if (m_dwNumVert < m_dwNumUV)//Direct3D�ł�UV�̐��������_���K�v
	{
		//���L���_���ŁA���_��������Ȃ���
		MessageBox(0, "Direct3D�́AUV�̐��������_���K�v�ł��iUV��u���ꏊ���K�v�ł��j�e�N�X�`���[�͐������\���Ȃ��Ǝv���܂�", NULL, MB_OK);
		return E_FAIL;
	}
	//�ꎞ�I�ȃ������m�ہi���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�j
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[m_dwNumVert];
	int* piFaceBuffer = new int[m_dwNumFace * 3];//�R���_�|���S���Ȃ̂ŁA1�t�F�C�X=3���_(3�C���f�b�N�X)

	//���_�ǂݍ���
	for (int i = 0; i < m_dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pD3dxMesh->GetVertexCoord(i);
		pvVB[i].vPos.x = v.x;
		pvVB[i].vPos.y = v.y;
		pvVB[i].vPos.z = v.z;
	}
	//�|���S�����i���_�C���f�b�N�X�j�ǂݍ���
	for (int i = 0; i < m_dwNumFace * 3; i++)
	{
		piFaceBuffer[i] = m_pD3dxMesh->GetIndex(i);
	}
	//�@���ǂݍ���
	for (int i = 0; i < m_dwNumVert; i++)
	{
		D3DXVECTOR3 v = m_pD3dxMesh->GetNormal(i);
		pvVB[i].vNorm.x = v.x;
		pvVB[i].vNorm.y = v.y;
		pvVB[i].vNorm.z = v.z;
	}
	//�e�N�X�`���[���W�ǂݍ���
	for (int i = 0; i < m_dwNumVert; i++)
	{
		D3DXVECTOR2 v = m_pD3dxMesh->GetUV(i);
		pvVB[i].vTex.x = v.x;
		pvVB[i].vTex.y = v.y;
	}
	//�}�e���A���ǂݍ���
	m_dwNumMaterial = m_pD3dxMesh->GetNumMaterials();
	m_pMaterial = new MY_SKINMATERIAL[m_dwNumMaterial];

	//�}�e���A���̐������C���f�b�N�X�o�b�t�@�[���쐬
	m_ppIndexBuffer = new ID3D11Buffer * [m_dwNumMaterial];
	for (int i = 0; i < m_dwNumMaterial; i++)
	{
		//����	
		m_pMaterial[i].Ka.x = m_pD3dxMesh->GetAmbient(i).y;
		m_pMaterial[i].Ka.y = m_pD3dxMesh->GetAmbient(i).z;
		m_pMaterial[i].Ka.z = m_pD3dxMesh->GetAmbient(i).w;
		m_pMaterial[i].Ka.w = m_pD3dxMesh->GetAmbient(i).x;
		//�g�U���ˌ�	
		m_pMaterial[i].Kd.x = m_pD3dxMesh->GetDiffuse(i).y;
		m_pMaterial[i].Kd.y = m_pD3dxMesh->GetDiffuse(i).z;
		m_pMaterial[i].Kd.z = m_pD3dxMesh->GetDiffuse(i).w;
		m_pMaterial[i].Kd.w = m_pD3dxMesh->GetDiffuse(i).x;
		//���ʔ��ˌ�
		m_pMaterial[i].Ks.x = m_pD3dxMesh->GetSpecular(i).y;
		m_pMaterial[i].Ks.y = m_pD3dxMesh->GetSpecular(i).z;
		m_pMaterial[i].Ks.z = m_pD3dxMesh->GetSpecular(i).w;
		m_pMaterial[i].Ks.w = m_pD3dxMesh->GetSpecular(i).x;

		//�e�N�X�`���[�i�f�B�t���[�Y�e�N�X�`���[�̂݁j
		char* name = m_pD3dxMesh->GetTexturePath(i);
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
		int* pIndex = new int[m_dwNumFace * 3];//�Ƃ肠�����A���b�V�����̃|���S�����Ń������m�ہi�X�̃|���S���O���[�v�͂��Ȃ炸����ȉ��ɂȂ邪�j

		for (int k = 0; k < m_dwNumFace; k++)
		{
			if (i == m_pD3dxMesh->GeFaceMaterialIndex(k))//���� i == k�Ԗڂ̃|���S���̃}�e���A���ԍ� �Ȃ�
			{
				pIndex[iCount] = m_pD3dxMesh->GetFaceVertexIndex(k, 0);//k�Ԗڂ̃|���S������钸�_�̃C���f�b�N�X�@1��
				pIndex[iCount + 1] = m_pD3dxMesh->GetFaceVertexIndex(k, 1);//2��
				pIndex[iCount + 2] = m_pD3dxMesh->GetFaceVertexIndex(k, 2);//3��
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
	bd.ByteWidth = sizeof(MY_SKINVERTEX) * m_dwNumVert;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;
	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&bd, &InitData, &m_pVertexBuffer)))
		return FALSE;

	//�ꎞ�I�ȓ��ꕨ�́A���͂�s�v
	delete piFaceBuffer;
	if (pvVB) delete pvVB;

	return S_OK;
}
//
//HRESULT CD3DXSKINMESH::CreateIndexBuffer(DWORD dwSize,int* pIndex,ID3D11Buffer** ppIndexBuffer)
//Direct3D�̃C���f�b�N�X�o�b�t�@�[�쐬
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
		return FALSE;
	}

	return S_OK;
}

//
//void CD3DXSKINMESH::SetNewPoseMatrices(int frame)
//�{�[�������̃|�[�Y�ʒu�ɃZ�b�g����
void SkinMesh::SetNewPoseMatrices(int iFrame)
{
	//�]�ރt���[����Update���邱�ƁB���Ȃ��ƍs�񂪍X�V����Ȃ�
	//m_pD3dxMesh->UpdateFrameMatrices(m_pD3dxMesh->m_pFrameRoot)�������_�����O���Ɏ��s���邱��

	//�܂��A�A�j���[�V�������ԂɌ��������s����X�V����̂�D3DXMESH�ł�
	//�A�j���[�V�����R���g���[���[���i���Łj����Ă������̂Ȃ̂ŁA�A�j���[�V�����R���g���[���[
	//���g���ăA�j����i�s�����邱�Ƃ��K�v
	//	m_pD3dxMesh->m_pAnimController->AdvanceTime(....)�������_�����O���Ɏ��s���邱��

	for (int i = 0; i < m_iNumBone; i++)
	{
		m_BoneArray[i].mNewPose = m_pD3dxMesh->GetNewPose(i);
	}
}
//
//D3DXMATRIX CD3DXSKINMESH::GetCurrentPoseMatrix(int index)
//���́i���݂́j�|�[�Y�s���Ԃ�
D3DXMATRIX SkinMesh::GetCurrentPoseMatrix(int index)
{
	D3DXMATRIX ret = m_BoneArray[index].mBindPose * m_BoneArray[index].mNewPose;
	return ret;
}
//
//D3DXMATRIX CD3DXSKINMESH::GetBindPoseMatrix(int index)
//�o�C���h�|�[�Y�s���Ԃ�
D3DXMATRIX SkinMesh::GetBindPoseMatrix(int index)
{
	return m_BoneArray[index].mBindPose;
}
//
//
//
CHAR* SkinMesh::GetBoneNames(int iBoneIndex)
{
	return m_pD3dxMesh->GetBoneName(iBoneIndex);
}
