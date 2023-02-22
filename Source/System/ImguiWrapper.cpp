/**
* @file ImguiWrapper.h
* @brief Imgui�̐ÓI�N���X
* @author shiihara_kiyosumi
* @date 2022_12_20
*/

#include "ImguiWrapper.h"
#include "Game/Component/Image/Image.h"



// �ÓI�����o�ϐ��̒�`
int ImguiWrapper<Image>::idCount = 0;

std::map<ImguiWrapper<Image>::Mode, std::function<void(ImguiWrapper<Image>::Setting&)>> ImguiWrapper<Image>::funcs = { {ImguiWrapper<Image>::Mode::All, AllMode},{ImguiWrapper<Image>::Mode::Scale, ScaleMode},{ImguiWrapper<Image>::Mode::Rotate, RotateMode},{ImguiWrapper<Image>::Mode::Pos, PosMode} };

