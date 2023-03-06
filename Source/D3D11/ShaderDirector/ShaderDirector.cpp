/**
* @file ShaderDirector.h
* @brief �V�F�[�_�[�̊Ǘ�
* @details ShaderDirector�N���X�̓V���O���g���Ȃ̂Œ���
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "ShaderDirector.h"
#include "System/Common.h"
#include <cassert>
#include "System/Vector3D.h"



// �N���X�̐ÓI�ϐ��̏�����
ShaderDirector* ShaderDirector::m_this = nullptr;





/**
* @fn CreateShader
* @brief �V�F�[�_�[�̍쐬
* @param[in] kind ��肽���V�F�[�_�[�̎��
* @return HRESULT S_OK�Ő���
*/
HRESULT ShaderDirector::CreateShader(const SHADER_KIND kind)
{
	assert(!m_this->m_shaderVariableArray[kind].isAlive);
	switch (kind)
	{
	case PRIMITIVE:
		m_this->CreatePrimitiveShader();
		break;
	case PRIMITIVE2D:
		m_this->CreatePrimitive2DShader();
		break;
	default:
		return E_FAIL;
		break;
	}

	return S_OK;
}

/**
* @fn ReleaseShader
* @brief �V�F�[�_�̍폜
* @param[in] kind ���������V�F�[�_�[�̎��
*/
void ShaderDirector::ReleaseShader(const SHADER_KIND kind)
{
	assert(m_this->m_shaderVariableArray[kind].isAlive);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pConstantBuffer);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pPixelShader);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pInputLayout);
	SAFE_RELEASE(m_this->m_shaderVariableArray[kind].pVertexShader);
	m_this->m_shaderVariableArray[kind].isAlive = false;
}

/**
* @fn ShaderDirector
* @brief �R���X�g���N�^
*/
ShaderDirector::ShaderDirector()
{
	for (int i = 0; i < SHADER_KIND_MAX; i++)
	{
		m_shaderVariableArray[i].pConstantBuffer = NULL;
		m_shaderVariableArray[i].pPixelShader = NULL;
		m_shaderVariableArray[i].pInputLayout = NULL;
		m_shaderVariableArray[i].pVertexShader = NULL;
		m_shaderVariableArray[i].isAlive = false;
	}
}

/**
* @fn ~ShaderDirector
* @brief �f�X�g���N�^
*/
ShaderDirector::~ShaderDirector()
{
	for (int i = 0; i < SHADER_KIND_MAX; i++)
	{
		SAFE_RELEASE(m_shaderVariableArray[i].pConstantBuffer);
		SAFE_RELEASE(m_shaderVariableArray[i].pPixelShader);
		SAFE_RELEASE(m_shaderVariableArray[i].pInputLayout);
		SAFE_RELEASE(m_shaderVariableArray[i].pVertexShader);
		m_shaderVariableArray[i].isAlive = false;
	}
}

/**
* @fn CreatePrimitiveShader
* @brief �v���~�e�B�u�̃V�F�[�_�[�̍쐬
* @return HRESULT S_OK
*/
HRESULT ShaderDirector::CreatePrimitiveShader()
{
	// �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob* pCompiledShader = NULL;

	// �o�[�e�b�N�X�V�F�[�_�[�쐬
	//m_this->CreateVS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitiveVS.hlsl", PRIMITIVE);
	D3D11::CreateVertexShader(&m_shaderVariableArray[PRIMITIVE].pVertexShader, &pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitiveVS.hlsl", "VS");

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof(layout) / sizeof(layout[0]);
	D3D11::CreateInputLayout(&m_shaderVariableArray[PRIMITIVE].pInputLayout, &pCompiledShader, layout, numElements);
	//m_this->CreateIL(&pCompiledShader, layout, numElements, PRIMITIVE);
	SAFE_RELEASE(pCompiledShader);

	// �s�N�Z���V�F�[�_�[�쐬
	//m_this->CreatePS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitivePS.hlsl", PRIMITIVE);
	D3D11::CreatePixelShader(&m_shaderVariableArray[PRIMITIVE].pPixelShader, &pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitivePS.hlsl", "PS");
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11::CreateConstantBuffer(&m_shaderVariableArray[PRIMITIVE].pConstantBuffer, sizeof(PrimitiveConstantBuffer));
	//m_this->CreateCB(sizeof(PrimitiveConstantBuffer), PRIMITIVE);
	
	return S_OK;
}

/**
* @fn CreatePrimitive2DShader
* @brief �v���~�e�B�u2D�̃V�F�[�_�[�̍쐬
* @return HRESULT S_OK
*/
HRESULT ShaderDirector::CreatePrimitive2DShader()
{
	// �u���u�쐬�@�u���u�Ƃ̓V�F�[�_�[�̉�݂����Ȃ��́BXX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	ID3DBlob* pCompiledShader = NULL;

	// �o�[�e�b�N�X�V�F�[�_�[�쐬
	if (FAILED(m_this->CreateVS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitiveVS2D.hlsl", PRIMITIVE2D)))
	{
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	m_this->CreateIL(&pCompiledShader, layout, 1, PRIMITIVE2D);

	SAFE_RELEASE(pCompiledShader);

	// �s�N�Z���V�F�[�_�[�쐬
	if (FAILED(m_this->CreatePS(&pCompiledShader, "Source/Game/Component/Primitive/Shader/PrimitivePS2D.hlsl", PRIMITIVE2D)))
	{
		return E_FAIL;
	}
	SAFE_RELEASE(pCompiledShader);

	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	m_this->CreateCB(sizeof(PrimitiveConstantBuffer2D), PRIMITIVE2D);
	
	return S_OK;
}

/**
* @fn CreateVS
* @brief �o�[�e�b�N�X�V�F�[�_�[�̍쐬
* @param[out] compiledShader XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
* @param[in] fileName �V�F�[�_�[�̃t�@�C����
* @param[in] kind �V�F�[�_�[�̎��
* @return HRESULT ������S_OK
*/
HRESULT ShaderDirector::CreateVS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind)
{
	auto& comShader = *compiledShader;
	ID3DBlob* pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile(fileName, NULL, NULL, "VS", "vs_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &comShader, &pErrors, NULL)))
	{
		char* p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreateVertexShader(comShader->GetBufferPointer(), comShader->GetBufferSize(), NULL, (ID3D11VertexShader**)&m_this->m_shaderVariableArray[kind].pVertexShader)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
* @fn CreatePS
* @brief �s�N�Z���V�F�[�_�[�̍쐬
* @param[out] compiledShader XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
* @param[in] fileName �V�F�[�_�[�̃t�@�C����
* @param[in] kind �V�F�[�_�[�̎��
* @return HRESULT ������S_OK
*/
HRESULT ShaderDirector::CreatePS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind)
{
	auto& comShader = *compiledShader;
	ID3DBlob* pErrors = NULL;

	if (FAILED(D3DX11CompileFromFile(fileName, NULL, NULL, "PS", "ps_5_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &comShader, &pErrors, NULL)))
	{
		char* p = (char*)pErrors->GetBufferPointer();
		MessageBoxA(0, p, 0, MB_OK);
		return E_FAIL;
	}
	SAFE_RELEASE(pErrors);
	if (FAILED(Direct3D11::GetDevice()->CreatePixelShader(comShader->GetBufferPointer(), comShader->GetBufferSize(), NULL, (ID3D11PixelShader**)&m_this->m_shaderVariableArray[kind].pPixelShader)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
* @fn CreateCB
* @brief �R���X�^���g�o�b�t�@�[�̍쐬
* @param[in] constBufferSize �R���X�^���g�o�b�t�@�[�̃T�C�Y(sizeof(STRUCT)�݂����Ȋ���)
* @param[in] kind �V�F�[�_�[�̎��
* @return HRESULT ������S_OK
*/
HRESULT ShaderDirector::CreateCB(UINT constBufferSize, const SHADER_KIND kind)
{
	//�R���X�^���g�o�b�t�@�[�쐬�@�����ł͕ϊ��s��n���p
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = constBufferSize;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;

	if (FAILED(Direct3D11::GetDevice()->CreateBuffer(&cb, NULL, &m_this->m_shaderVariableArray[kind].pConstantBuffer)))
	{
		return E_FAIL;
	}
	return S_OK;
}

/**
* @fn CreateIL
* @brief InputLayout�̍쐬
* @param[in] compiledShader XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
* @param[in] layoutArray layout�̔z��
* @param[in] elementCount �z��̗v�f��
* @param[in] kind �V�F�[�_�[�̎��
* @return HRESULT ������S_OK
*/
HRESULT ShaderDirector::CreateIL(ID3DBlob** compiledShader, D3D11_INPUT_ELEMENT_DESC* layoutArray, int elementCount, const SHADER_KIND kind)
{
	auto& comShader = *compiledShader;
	//���_�C���v�b�g���C�A�E�g���쐬
	if (FAILED(Direct3D11::GetDevice()->CreateInputLayout(layoutArray, elementCount, comShader->GetBufferPointer(), comShader->GetBufferSize(), &m_this->m_shaderVariableArray[kind].pInputLayout)))
	{
		return E_FAIL;
	}
	return S_OK;
}
