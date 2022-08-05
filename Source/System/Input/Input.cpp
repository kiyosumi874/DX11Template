/**
* @file Input.cpp
* @brief �L�[�{�[�h��R���g���[���[�̓���
* @details Input�N���X�̓V���O���g���Ȃ̂Œ���,�R���g���[��������
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// �w�b�_�[�t�@�C���̃C���N���[�h
#include "Input.h"
#include <Windows.h>
#include <vector>

// using�錾
using std::vector;

// �N���X�̐ÓI�ϐ��̏�����
Input* Input::m_this = nullptr;

/**
* @fn Input
* @brief �R���X�g���N�^
*/
Input::Input()
{
	// m_keys�������p�̔z��
	vector<int> initKeyCode[2];
	
	// �L�[�{�[�h�̉��z�L�[�R�[�h���Z�b�g
	initKeyCode[KEYBOARD].push_back(VK_DOWN);
	initKeyCode[KEYBOARD].push_back(VK_LEFT);
	initKeyCode[KEYBOARD].push_back(VK_RIGHT);
	initKeyCode[KEYBOARD].push_back(VK_UP);
	initKeyCode[KEYBOARD].push_back(static_cast<int>('X'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('V'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('Z'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('C'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('B'));
	initKeyCode[KEYBOARD].push_back(static_cast<int>('N'));
	initKeyCode[KEYBOARD].push_back(VK_ESCAPE);
	initKeyCode[KEYBOARD].push_back(VK_RETURN);

	// �R���g���[���[�̉��z�L�[�R�[�h���Z�b�g
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_DOWN);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_LEFT);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_RIGHT);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_UP);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_A);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_B);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_X);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_Y);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_L);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_R);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_BACK);
	initKeyCode[CONTROLLER_1P].push_back(PAD_ID_START);

	// m_keys�ɃR�s�[
	for (int i = 0; i < BUTTON_ID_MAX; i++)
	{
		m_keys[KEYBOARD][i].keyCode = initKeyCode[KEYBOARD][i];
		m_keys[KEYBOARD][i].pressCount = -1;
		m_keys[CONTROLLER_1P][i].keyCode = initKeyCode[CONTROLLER_1P][i];
		m_keys[CONTROLLER_1P][i].pressCount = -1;
	}
}

/**
* @fn ~Input
* @brief �f�X�g���N�^
*/
Input::~Input()
{
	// ���邱�ƂȂ��̂Ŏ������ĂȂ�
}

/**
* @fn Update
* @brief �X�V
* @attention �P���[�v�Ɉ��K���Ăяo������
*/
void Input::Update()
{
	for (int i = 0; i < BUTTON_ID_MAX; i++)
	{
		// �L�[�{�[�h�̃A�b�v�f�[�g
		// GetAsyncKeyState�͉����Ă���Ƃ��ŏ�ʃr�b�g������
		if (GetAsyncKeyState(m_this->m_keys[KEYBOARD][i].keyCode) & 0x8000) // 0x8000��short�̍ŏ�ʃr�b�g�������Ă��邱�Ƃ�\��
		{
			m_this->m_keys[KEYBOARD][i].pressCount = max(++m_this->m_keys[KEYBOARD][i].pressCount, 1);
		}
		else
		{
			m_this->m_keys[KEYBOARD][i].pressCount = min(--m_this->m_keys[KEYBOARD][i].pressCount, 0);
		}

		// �R���g���[���[1P�̃A�b�v�f�[�g
		/*if (GetJoypadInputState(DX_INPUT_PAD1) & m_input->m_keys[CONTROLLER_1P][i].keyCode)
		{
			m_input->m_keys[CONTROLLER_1P][i].pressCount = max(++m_input->m_keys[CONTROLLER_1P][i].pressCount, 1);
		}
		else
		{
			m_input->m_keys[CONTROLLER_1P][i].pressCount = min(--m_input->m_keys[CONTROLLER_1P][i].pressCount, 0);
		}*/

	}
}
