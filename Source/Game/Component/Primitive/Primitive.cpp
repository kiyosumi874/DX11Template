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

		// アフィン変換
		{
			D3DXMATRIX scale;
			D3DXMATRIX rotate;
			D3DXMATRIX pos;

			D3DXMatrixIdentity(&world); // 行列の初期化

			D3DXMatrixScaling(&scale, m_transform->scale.x, m_transform->scale.y, m_transform->scale.z);
			D3DXMatrixRotationYawPitchRoll(&rotate, m_transform->rotation.x, m_transform->rotation.y, m_transform->rotation.z);
			//D3DXMatrixRotationY(&rotate, timeGetTime() / 1000.0f);
			D3DXMatrixTranslation(&pos, m_transform->position.x, m_transform->position.y, m_transform->position.z);

			// DirectXは行優先なのでScaleから乗算
			D3DXMatrixMultiply(&world, &world, &scale);
			D3DXMatrixMultiply(&world, &world, &rotate);
			D3DXMatrixMultiply(&world, &world, &pos);
		}
		


		CameraData data;
		ZeroMemory(&data, sizeof(CameraData));
		bool isSuccess = TellCameraData::GetCameraData(&data, CAMERA_NUMBER::CAMERA_0);

		if (!isSuccess)
		{
			// エラーメッセージ
			MyOutputDebugString("カメラ取得失敗");
		}

		//ワールド、ビュー、射影行列を渡す
		D3DXMATRIX m = world * data.matrixView * data.matrixProj;
		// なぜD3DXMatrixTransposeをしなきゃならないの？
		// ->DirectXは行優先HLSLは列優先だからだ！
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
		MyOutputDebugString("マップに失敗");
	}
	//このコンスタントバッファーをどのシェーダーで使うか
	Direct3D11::GetDeviceContext()->VSSetConstantBuffers(0, 1, &shaderVar.pConstantBuffer);
	Direct3D11::GetDeviceContext()->PSSetConstantBuffers(0, 1, &shaderVar.pConstantBuffer);
	//頂点インプットレイアウトをセット
	Direct3D11::GetDeviceContext()->IASetInputLayout(shaderVar.pInputLayout);
	
}

