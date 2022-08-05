/**
* @file Fps.h
* @brief Fps�̌Œ�
* @details Fps�N���X�̓V���O���g���Ȃ̂Œ���
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#pragma once
#include <Windows.h>

/**
* @class Fps
* @brief Fps���Œ肷��N���X(�V���O���g��)
*/
class Fps
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
			m_this = new Fps;
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
	* @fn Update
	* @brief �X�V
	* @attention �P���[�v�Ɉ��K���Ăяo������
	*/
	static void Update();

	/**
	* @fn Draw
	* @brief �o��(VisualStudio)�Ɍ��݂�Fps��`��
	*/
	static void Draw();

private:
	/**
	* @fn Fps
	* @brief �R���X�g���N�^
	*/
	Fps();

	/**
	* @fn ~Fps
	* @brief �f�X�g���N�^
	*/
	~Fps();


	float m_fps; // ����fps
	float m_frameTime; // 1���[�v�̌o�ߎ���
	LARGE_INTEGER m_timeStart; // 1���[�v�̍ŏ��Ɍv����������
	LARGE_INTEGER m_timeEnd; // 1���[�v�̍Ō�Ɍv����������
	LARGE_INTEGER m_timeFreq; // ���g��

	static Fps* m_this; // �ÓI�Ȏ����̃|�C���^

};