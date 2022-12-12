// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Primitive.h"
#include "System/Common.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Component/Object.h"
#include "System/Transform.h"



Primitive::Primitive()
	: m_pVertexBuffer(NULL)
	, m_is2D(false)
	, m_transform(nullptr)
{
}

Primitive::~Primitive()
{
	SAFE_RELEASE(m_pVertexBuffer);
}

/**
* @fn DrawCommon
* @brief Primitive���ʂ̕`��
*/
void Primitive::DrawCommon()
{
	SHADER_KIND kind = PRIMITIVE;
	if (m_is2D)
	{
		kind = PRIMITIVE2D;
	}
	auto shaderVar = ShaderDirector::GetShaderVariable(kind);
	//�g�p����V�F�[�_�[�̃Z�b�g
	Direct3D11::GetDeviceContext()->VSSetShader(shaderVar.pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(shaderVar.pPixelShader, NULL, 0);
	//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�[�Ɋe��f�[�^��n��
	D3D11_MAPPED_SUBRESOURCE pData;
	PrimitiveConstantBuffer cb;
	PrimitiveConstantBuffer2D cb2;

	if (SUCCEEDED(Direct3D11::GetDeviceContext()->Map(shaderVar.pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		D3DXMATRIX world;

		// �A�t�B���ϊ�
		{
			D3DXMATRIX scale;
			D3DXMATRIX rotate;
			D3DXMATRIX pos;

			D3DXMatrixIdentity(&world); // �s��̏�����

			D3DXMatrixScaling(&scale, m_transform->scale.x, m_transform->scale.y, m_transform->scale.z);
			D3DXMatrixRotationYawPitchRoll(&rotate, m_transform->rotation.x, m_transform->rotation.y, m_transform->rotation.z);
			//D3DXMatrixRotationY(&rotate, timeGetTime() / 1000.0f);
			D3DXMatrixTranslation(&pos, m_transform->position.x, m_transform->position.y, m_transform->position.z);

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

		//���[���h�A�r���[�A�ˉe�s���n��
		D3DXMATRIX m = world * data.matrixView * data.matrixProj;
		// �Ȃ�D3DXMatrixTranspose�����Ȃ���Ȃ�Ȃ��́H
		// ->DirectX�͍s�D��HLSL�͗�D�悾���炾�I
		D3DXMatrixTranspose(&m, &m);
		D3DXMatrixTranspose(&world, &world);
		cb.mWVP = m;
		cb.mW = world;
		D3DXVECTOR3 light(0, 0.5, -1);
		D3DXVec3Normalize(&light, &light);
		cb.lightDir = (D3DXVECTOR4)light;
		D3DXVECTOR4 color;
		color.x = m_color.x;
		color.y = m_color.y;
		color.z = m_color.z;
		color.w = 1.0f;
		cb.color = color;
		cb.eye = D3DXVECTOR4(data.pos.x, data.pos.y, data.pos.z, 0.0f);
		cb2.mW = world;
		cb2.viewPortWidth = WINDOW_WIDTH;
		cb2.viewPortHeight = WINDOW_HEIGHT;
		cb2.color = color;
		if (m_is2D)
		{
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb2), sizeof(cb2));
		}
		else
		{
			memcpy_s(pData.pData, pData.RowPitch, (void*)(&cb), sizeof(cb));
		}
		Direct3D11::GetDeviceContext()->Unmap(shaderVar.pConstantBuffer, 0);
	}
	else
	{
		MyOutputDebugString("�}�b�v�Ɏ��s");
	}
	//���̃R���X�^���g�o�b�t�@�[���ǂ̃V�F�[�_�[�Ŏg����
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &shaderVar.pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &shaderVar.pConstantBuffer);
	//���_�C���v�b�g���C�A�E�g���Z�b�g
	Direct3D11::GetDeviceContext()->IASetInputLayout(shaderVar.pInputLayout);
	
}

