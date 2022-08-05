/**
* @file main.cpp
* @brief ���s�����Ƃ��ɂ�������n�܂�
* @author shiihara_kiyosumi
* @date 2022_07_26
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include <Windows.h>
#include "System/Director/Director.h"

/** �G���g���[�֐� */
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	// �f�B���N�^�[�쐬
	Director* pDirector = new Director;

	if (!pDirector)
	{
		MessageBox(0, "�N���X�������s�@�A�v�����I�����܂�", NULL, MB_OK);
		return -1;
	}

	// �f�B���N�^�[������
	if (FAILED(pDirector->Init(hInst)))
	{
		MessageBox(0, "�N���X���������s�@�A�v�����I�����܂�", NULL, MB_OK);
		delete pDirector;
		return -1;
	}

	pDirector->Run();

	// �f�B���N�^�[�I��
	delete pDirector;
	return 0;
}