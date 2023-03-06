/**
* @file ImguiWrapper.h
* @brief Imgui�̐ÓI�N���X
* @author shiihara_kiyosumi
* @date 2022_12_20
*/

#include "ImguiWrapper.h"



// �ÓI�����o�ϐ��̒�`
int ImguiWrapper::idCount = 0;

std::map<ImguiWrapper::Mode, std::function<void(ImguiWrapper::Setting&)>> ImguiWrapper::funcs = { {ImguiWrapper::Mode::All, AllMode},{ImguiWrapper::Mode::Scale, ScaleMode},{ImguiWrapper::Mode::Rotate, RotateMode},{ImguiWrapper::Mode::Pos, PosMode} };

