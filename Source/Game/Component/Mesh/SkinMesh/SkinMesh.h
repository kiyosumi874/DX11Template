#pragma once
#include "Game/Component/Mesh/Mesh.h"
#include "Game/Component/Component.h"

class SkinMesh : public Component
{
public:
	//メソッド
	void Start()override;
	void Terminate()override;
	void Update()override;
	void Draw()override;
	HRESULT Init(LPCSTR fileName);
private:
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);
	HRESULT CreateFromX(LPCSTR szFileName);
	HRESULT ReadSkinInfo(MY_SKINVERTEX*);
	void SetNewPoseMatrices(int frame);
	D3DXMATRIX GetBindPoseMatrix(int index);
	D3DXMATRIX GetCurrentPoseMatrix(int index);
	CHAR* GetBoneNames(int iBoneIndex);

	//Dx11
	ID3D11SamplerState* m_pSampleLinear;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pConstantBufferBone;

	//メッシュ
	D3DXPARSER* m_pD3dxMesh;
	DWORD m_dwNumVert;
	DWORD m_dwNumFace;
	DWORD m_dwNumUV;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	MY_SKINMATERIAL* m_pMaterial;
	DWORD m_dwNumMaterial;

	//ボーン
	int m_iNumBone;
	BONE* m_BoneArray;

	
};