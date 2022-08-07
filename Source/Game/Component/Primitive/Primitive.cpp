// ヘッダーファイルのインクルード
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
* @brief Primitive共通の描画
*/
void Primitive::DrawCommon()
{
	SHADER_KIND kind = PRIMITIVE;
	if (m_is2D)
	{
		kind = PRIMITIVE2D;
	}
	auto shaderVar = ShaderDirector::GetShaderVariable(kind);
	//使用するシェーダーのセット
	Direct3D11::GetDeviceContext()->VSSetShader(shaderVar.pVertexShader, NULL, 0);
	Direct3D11::GetDeviceContext()->PSSetShader(shaderVar.pPixelShader, NULL, 0);
	//シェーダーのコンスタントバッファーに各種データを渡す
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
			// エラーメッセージ
			MyOutputDebugString("カメラ取得失敗");
		}

		//ワールド、カメラ、射影行列を渡す
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
		MyOutputDebugString("マップに失敗");
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &shaderVar.pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &shaderVar.pConstantBuffer);
	//頂点インプットレイアウトをセット
	Direct3D11::GetDeviceContext()->IASetInputLayout(shaderVar.pInputLayout);
	
}

