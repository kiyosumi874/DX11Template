/**
* @file Common.h
* @brief �}�N���̒�`�Ȃǂ��낢��
* @author shiihara_kiyosumi
* @date 2022_07_28
*/

// ��d�C���N���[�h�h�~
#pragma once
#include <stdio.h>

#define WINDOW_NAME "�e���v��" // ���̃A�v���P�[�V�����̖��O
#define WINDOW_WIDTH 640 // �E�B���h�E�̉���
#define WINDOW_HEIGHT 480 // �E�B���h�E�̏c��
const float MIN_FRAME_TIME = 1.0f / 60.0f;
// �֗��}�N������
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define MFAIL(code,string) if(FAILED( code ) ) { MessageBox(0,string,"error",MB_OK); return E_FAIL; }
#define MFALSE(code,string) if(!( code ) ) { MessageBox(0,string,"error",MB_OK); return E_FAIL; }
#define MSG(t) MessageBox(0,t,0,MB_OK);
#ifdef _DEBUG
#   define MyOutputDebugString( ... ) \
      { \
        TCHAR c[256]; \
        sprintf_s( c, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // �����
#endif