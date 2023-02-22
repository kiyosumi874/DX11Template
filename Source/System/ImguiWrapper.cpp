/**
* @file ImguiWrapper.h
* @brief Imguiの静的クラス
* @author shiihara_kiyosumi
* @date 2022_12_20
*/

#include "ImguiWrapper.h"
#include "Game/Component/Image/Image.h"



// 静的メンバ変数の定義
int ImguiWrapper<Image>::idCount = 0;

std::map<ImguiWrapper<Image>::Mode, std::function<void(ImguiWrapper<Image>::Setting&)>> ImguiWrapper<Image>::funcs = { {ImguiWrapper<Image>::Mode::All, AllMode},{ImguiWrapper<Image>::Mode::Scale, ScaleMode},{ImguiWrapper<Image>::Mode::Rotate, RotateMode},{ImguiWrapper<Image>::Mode::Pos, PosMode} };

