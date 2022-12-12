/**
* @file StaticMesh.h
* @brief StaticMesh�̕`��
* @author shiihara_kiyosumi
* @date 2022_08_17
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <d3dx9.h>
#include <d3d11.h>
#include "Game/Component/Component.h"
#include "System/Transform.h"
#include "System/Common.h"
#include <vector>

// using�錾
using std::vector;

struct ConstantBuffer0
{
	D3DXMATRIX world;//���[���h�s��
	D3DXMATRIX worldVewProj;//���[���h����ˉe�܂ł̕ϊ��s��
	D3DXVECTOR4 lightDir;//���C�g����
	D3DXVECTOR4 eye;//�J�����ʒu
};

struct ConstantBuffer1
{
	D3DXVECTOR4 ambient;//�A���r�G���g��
	D3DXVECTOR4 diffuse;//�f�B�t���[�Y�F
	D3DXVECTOR4 specular;//���ʔ���
};

//�I���W�i���@�}�e���A���\����
struct MyMaterial
{
	CHAR name[100];
	D3DXVECTOR4 ambient;//�A���r�G���g
	D3DXVECTOR4 diffuse;//�f�B�t���[�Y
	D3DXVECTOR4 specular;//�X�y�L�����[
	CHAR textureName[256];//�e�N�X�`���[�t�@�C����
	ID3D11ShaderResourceView* pTexture;
	DWORD numFace;//���̃}�e���A���ł���|���S����
	MyMaterial()
	{
		ZeroMemory(this, sizeof(MyMaterial));
	}
	~MyMaterial()
	{
		SAFE_RELEASE(pTexture);
	}
};

//���_�̍\����
struct MyVertex
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 norm;
	D3DXVECTOR2 tex;
};

/**
* @class StaticMesh
* @brief �����Ȃ����b�V���N���X
*/
class StaticMesh : public Component
{
public:
	/**
	* @fn Start
	* @brief ���������Ƃ��ɍŏ��Ɉ�񂾂�����֐�
	*/
	void Start() override;

	/**
	* @fn Update
	* @brief �X�V
	*/
	void Update() override;

	/**
	* @fn Draw
	* @brief �`��
	*/
	void Draw() override;

	/**
	* @fn Terminate
	* @brief �I������
	*/
	void Terminate() override;

	/**
	* @fn Init
	* @brief ������
	*/
	HRESULT Init(const char* fileName);

private:
	/**
	* @fn LoadXMesh
	* @brief xfile�����[�h����
	* @param[in] fileName �t�@�C���̖��O
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT LoadXMesh(const char* fileName);

	HRESULT FetchMaterial();

	HRESULT CreateIndexBuffer();

	HRESULT CreateVertexBuffer();

	HRESULT CreateSampler();

	LPD3DXMESH m_pMesh; // ���b�V��
	LPD3DXBUFFER m_pD3DXMtrlBuffer;
	DWORD m_numMaterial; // �}�e���A���̐�

	DWORD m_numAttribute; // ������
	DWORD m_attributeID[300]; // 300�����܂�

	// resource
	ID3D11SamplerState* m_pSampler;//�e�N�X�`���[�̃T���v���[
	vector<MyMaterial> m_materialVec;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pVertexBuffer;
	vector<ID3D11Buffer*> m_pIndexBufferVec;
	bool m_useTexture;
};
