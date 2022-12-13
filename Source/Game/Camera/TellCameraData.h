/**
* @file TellCameraData.h
* @brief �����̃J�����̏���`����(�V���O���g��)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <unordered_map>
#include "CameraStruct.h"
#include "System/Common.h"

// using�錾
using std::unordered_map;

/**
* @class TellCameraData
* @brief �����̃J�����̏���`����N���X(�V���O���g��)
*/
class TellCameraData
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
			m_this = new TellCameraData;
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
	* @fn AddCamera
	* @brief �J�����̏���ǉ�
	* @param[in] data �J�����̏��
	*/
	static void AddCamera(const CameraData& data)
	{
		m_this->m_cameraDataVec[data.key] = data;
	}

	/**
	* @fn SubCamera
	* @brief �J�����̏����폜
	* @param[in] data �J�����̏��
	*/
	static bool SubCamera(const CAMERA_NUMBER key)
	{
		auto& cameraDataVec = m_this->m_cameraDataVec;

		if (cameraDataVec.empty())
		{
			MSG("Camera�͌��ݓo�^����Ă��܂���")
			return false;
		}

		cameraDataVec.erase(key);
		return false;
	}

	/**
	* @fn GetCameraData
	* @brief �J�����̏����擾
	* @param[out] cameraData ����ɃJ�����̃f�[�^��f���o��
	* @param[in] key �X�J������ID
	* @return bool true�Ő���
	*/
	static bool GetCameraData(CameraData* cameraData, CAMERA_NUMBER key)
	{
		auto& cameraDataVec = m_this->m_cameraDataVec;
		*cameraData = cameraDataVec[key];
		return false;
	}


private:
	/**
	* @fn Fps
	* @brief �R���X�g���N�^
	*/
	TellCameraData();

	/**
	* @fn ~Fps
	* @brief �f�X�g���N�^
	*/
	~TellCameraData();

	unordered_map<CAMERA_NUMBER,CameraData> m_cameraDataVec; // �J�����̏���ۑ�����z��

	static TellCameraData* m_this; // �ÓI�Ȏ����̃N���X�̃|�C���^

};