// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Primitive.h"
#include "System/Common.h"
#include "D3D11/Direct3D11.h"
#include "D3D11/ShaderDirector/ShaderDirector.h"
#include "Game/Camera/TellCameraData.h"
#include "Game/Component/Object.h"
#include "Game/Component/Transform/Transform.h"


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
		D3DXMatrixTranslation(&world, m_transform->m_position.x, m_transform->m_position.y, m_transform->m_position.z);

		CameraData data;
		ZeroMemory(&data, sizeof(CameraData));
		bool isSuccess = TellCameraData::GetCameraData(&data, CAMERA_NUMBER::CAMERA_0);

		if (!isSuccess)
		{
			// �G���[���b�Z�[�W
			MyOutputDebugString("�J�����擾���s");
		}

		//���[���h�A�J�����A�ˉe�s���n��
		D3DXMATRIX m = world * data.matrixView * data.matrixProj;
		D3DXMatrixTranspose(&m, &m);
		D3DXMatrixTranspose(&world, &world);
		cb.mWVP = m;
		cb2.mW = world;
		cb2.viewPortWidth = WINDOW_WIDTH;
		cb2.viewPortHeight = WINDOW_HEIGHT;
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

