/**
* @file SkinMesh.h
* @brief SkinMesh�̕`��(�܂����S�ł͂Ȃ�)
* @author shiihara_kiyosumi
* @date 2022_08_18
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include "Game/Component/Mesh/Mesh.h"
#include "Game/Component/Component.h"

/**
* @class SkinMesh
* @brief �{�[���t���̃A�j���[�V�����ł���
*/
class SkinMesh : public Component
{
public:
	/**
	* @fn Start
	* @brief �ŏ��Ɉ�x�����s������
	*/
	void Start()override;

	/**
	* @fn Terminate
	* @brief �Ō�Ɉ�x�����s������
	*/
	void Terminate()override;

	/**
	* @fn Update
	* @brief �X�V
	*/
	void Update()override;

	/**
	* @fn Draw
	* @brief �`��
	*/
	void Draw()override;

	/**
	* @fn Init
	* @breif ������
	* @param[in] fileName ���f���̃t�@�C����
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT Init(LPCSTR fileName);
private:
	/**
	* @fn CreateIndexBuffer
	* @brief �C���f�b�N�X�o�b�t�@�[�쐬
	* @param[in] dwSize �o�b�t�@�[�T�C�Y
	* @param[in] pIndex �������f�[�^�ւ̃|�C���^�[
	* @param[out, optional] ppIndexBuffer �쐬���ꂽ�o�b�t�@�[�I�u�W�F�N�g��ID3D11Buffer�C���^�[�t�F�C�X�ւ̃|�C���^�[�̃A�h���X
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);

	/**
	* @fn CreateFromX
	* @brief xfile����X�L�����b�V�����쐬����
	* @param[in] szFileName ���f���̃t�@�C����
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT CreateFromX(LPCSTR szFileName);

	/**
	* @fn ReadSkinInfo
	* @brief xfile����X�L���֘A�̏���ǂݏo��
	* @param[in] pvVB ���_�f�[�^�̍\���̂̃|�C���^
	* @return HRESULT S_OK�Ő���
	*/
	HRESULT ReadSkinInfo(MY_SKINVERTEX* pvVB);

	/**
	* @fn SetNewPoseMatrices
	* @brief �{�[�������̃|�[�Y�ʒu�ɃZ�b�g����
	* @return HRESULT S_OK�Ő���
	*/
	void SetNewPoseMatrices();

	/**
	* @fn GetBindPoseMatrix
	* @brief �o�C���h�|�[�Y�s���Ԃ�
	* @param[in] index �{�[���z��̓Y����
	* @return D3DXMATRIX �o�C���h�|�[�Y�s��
	*/
	D3DXMATRIX GetBindPoseMatrix(int index);

	/**
	* @fn GetCurrentPoseMatrix
	* @brief ����(���݂�)�|�[�Y�s���Ԃ�
	* @param[in] index �{�[���z��̓Y����
	* @return D3DXMATRIX ����(���݂�)�|�[�Y�s��
	*/
	D3DXMATRIX GetCurrentPoseMatrix(int index);

	/**
	* @fn GetBoneNames
	* @brief �{�[���̖��O�擾
	* @param[in] iBoneIndex �{�[���z��̓Y����
	* @return CHAR* �{�[���̖��O
	*/
	CHAR* GetBoneNames(int iBoneIndex);

	//Dx11
	ID3D11SamplerState* m_pSampler; // texture��\��̂Ɏg�����肷��
	ID3D11VertexShader* m_pVertexShader; // ���_�V�F�[�_�[���Ǘ�����
	ID3D11PixelShader* m_pPixelShader; // �s�N�Z���V�F�[�_�[���Ǘ�����
	ID3D11InputLayout* m_pVertexLayout; // ���_�f�[�^���O���t�B�b�N�X�p�C�v���C����InputAssembler(IA)�X�e�[�W�ɋ���������@�̒�`��ێ�����
	ID3D11Buffer* m_pConstantBufferLight; // �V�F�[�_�[�ɓn���萔(SHADER_SKIN_GLOBAL0)
	ID3D11Buffer* m_pConstantBufferWVPMaterial; // �V�F�[�_�[�ɓn���萔(SHADER_SKIN_GLOBAL1)
	ID3D11Buffer* m_pConstantBufferBone; // �V�F�[�_�[�ɓn���萔(SHADER_GLOBAL_BONES)

	//���b�V��
	D3DXPARSER* m_pMeshParser; // xfile�̉�̓N���X
	ID3D11Buffer* m_pVertexBuffer; // ���_�f�[�^���i�[����o�b�t�@
	ID3D11Buffer** m_ppIndexBuffer; // ���_�̕��я����i�[����o�b�t�@
	MY_SKINMATERIAL* m_pMaterial; // �}�e���A���̍\����
	DWORD m_dwNumMaterial; // �}�e���A���̐�

	// �{�[���֘A
	int m_iNumBone; // �{�[���̐�
	BONE* m_BoneArray; // �{�[���\���̂̓��I�z��
};