/**
* @file ShaderDirector.h
* @brief �V�F�[�_�[�̊Ǘ�
* @details ShaderDirector�N���X�̓V���O���g���Ȃ̂Œ���
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ���d�C���N���[�h�h�~
#pragma once
#include <D3D11.h>
#include <D3DX10.h>

// enum�ɂ��x��������
#pragma warning(disable:26812)

// ���ƂŃ��b�V���Ȃǂ̃N���X�Ɉړ��������ȁ[
/**
* @struct PrimitiveVertex
* @brief ���b�V��
*/
struct PrimitiveVertex
{
	D3DXVECTOR3 pos;
};

/**
* @struct PrimitiveConstantBuffer
* @brief �v���~�e�B�u�̒萔�o�b�t�@�[
*/
struct PrimitiveConstantBuffer
{
	D3DXMATRIX mWVP; // matrixWorldViewProjection
};

/**
* @struct PrimitiveConstantBuffer2D
* @brief 2D�v���~�e�B�u�̒萔�o�b�t�@�[
*/
struct PrimitiveConstantBuffer2D
{
	alignas(16) D3DXMATRIX mW;
	alignas(16) float viewPortWidth;
	alignas(16) float viewPortHeight;
};

/**
* @struct ShaderVariable
* @brief �V�F�[�_�[�̎��
*/
struct ShaderVariable
{
	ID3D11InputLayout* pInputLayout;
	ID3D11VertexShader* pVertexShader;
	ID3D11PixelShader* pPixelShader;
	ID3D11Buffer* pConstantBuffer;
	bool isAlive;
};

/**
* @enum SHADER_KIND
* @brief �V�F�[�_�[�̎��
*/
enum SHADER_KIND
{
	PRIMITIVE,
	PRIMITIVE2D,

	SHADER_KIND_MAX
};

/**
* @class ShaderDirector
* @brief �V�F�[�_�[�̊Ǘ��N���X(�V���O���g��)
*/
class ShaderDirector
{
public:
	
	/**
	* @fn CreateInstance
	* @brief �����̃C���X�^���X�𐶐�
	*/
	static void CreateInstance()
	{
		if (!m_this)
		{
			m_this = new ShaderDirector;
		}
	}

	/**
	* @fn DeleteInstance
	* @brief �����̃C���X�^���X���폜
	*/
	static void DeleteInstance()
	{
		if (m_this)
		{
			delete m_this;
			m_this = nullptr;
		}
	}
	 /**
	 * @fn GetShaderVariable
	 * @brief �V�F�[�_�[�Ŏg���ϐ��̎擾
	 * @param[in] kind �g�������V�F�[�_�[�̎��
	 * @return ShaderVariable �V�F�[�_�[�Ŏg���ϐ�
	 */
	static const ShaderVariable GetShaderVariable(const SHADER_KIND kind)
	{
		for (int i = 0; i < SHADER_KIND_MAX; i++)
		{
			if (kind != i)
			{
				continue;
			}
			return m_this->m_shaderVariableArray[i];
		}
		return m_this->m_shaderVariableArray[0];
	}

	/**
	* @fn CreateShader
	* @brief �V�F�[�_�[�̍쐬
	* @param[in] kind ��肽���V�F�[�_�[�̎��
	* @return HRESULT S_OK�Ő���
	*/
	static HRESULT CreateShader(const SHADER_KIND kind);

	/**
	* @fn ReleaseShader
	* @brief �V�F�[�_�̍폜
	* @param[in] kind ���������V�F�[�_�[�̎��
	*/
	static void ReleaseShader(const SHADER_KIND kind);

private:
	/**
	* @fn ShaderDirector
	* @brief �R���X�g���N�^
	*/
	ShaderDirector();

	/**
	* @fn ~ShaderDirector
	* @brief �f�X�g���N�^
	*/
	~ShaderDirector();

	/**
	* @fn CreatePrimitiveShader
	* @brief �v���~�e�B�u�̃V�F�[�_�[�̍쐬
	* @return HRESULT S_OK
	*/
	HRESULT CreatePrimitiveShader();

	/**
	* @fn CreatePrimitive2DShader
	* @brief �v���~�e�B�u2D�̃V�F�[�_�[�̍쐬
	* @return HRESULT S_OK
	*/
	HRESULT CreatePrimitive2DShader();

	/**
	* @fn CreateVS
	* @brief �o�[�e�b�N�X�V�F�[�_�[�̍쐬
	* @param[out] compiledShader XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	* @param[in] fileName �V�F�[�_�[�̃t�@�C����
	* @param[in] kind �V�F�[�_�[�̎��
	* @return HRESULT ������S_OK
	*/
	HRESULT CreateVS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind);
	
	/**
	* @fn CreatePS
	* @brief �s�N�Z���V�F�[�_�[�̍쐬
	* @param[out] compiledShader XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	* @param[in] fileName �V�F�[�_�[�̃t�@�C����
	* @param[in] kind �V�F�[�_�[�̎��
	* @return HRESULT ������S_OK
	*/
	HRESULT CreatePS(ID3DBlob** compiledShader, LPCSTR fileName, const SHADER_KIND kind);
	
	/**
	* @fn CreateCB
	* @brief �R���X�^���g�o�b�t�@�[�̍쐬
	* @param[in] constBufferSize �R���X�^���g�o�b�t�@�[�̃T�C�Y(sizeof(STRUCT)�݂����Ȋ���)
	* @param[in] kind �V�F�[�_�[�̎��
	* @return HRESULT ������S_OK
	*/
	HRESULT CreateCB(UINT constBufferSize, const SHADER_KIND kind);
	
	/**
	* @fn CreateIL
	* @brief InputLayout�̍쐬
	* @param[in] compiledShader XX�V�F�[�_�[�Ƃ��ē����������Ȃ��B��Ŋe��V�F�[�_�[�ɐ��蓾��B
	* @param[in] layoutArray layout�̔z��
	* @param[in] elementCount �z��̗v�f��
	* @param[in] kind �V�F�[�_�[�̎��
	* @return HRESULT ������S_OK
	*/
	HRESULT CreateIL(ID3DBlob** compiledShader, D3D11_INPUT_ELEMENT_DESC* layoutArray, int elementCount, const SHADER_KIND kind);

	ShaderVariable m_shaderVariableArray[SHADER_KIND_MAX]; // �V�F�[�_�[�Ɏg���ϐ��̔z��

	static ShaderDirector* m_this; // �ÓI�Ȏ����̃|�C���^
};

