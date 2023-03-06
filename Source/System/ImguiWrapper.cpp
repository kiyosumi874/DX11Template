/**
* @file ImguiWrapper.h
* @brief Imguiの静的クラス
* @author shiihara_kiyosumi
* @date 2022_12_20
*/

#include "ImguiWrapper.h"



// 静的メンバ変数の定義
int ImguiWrapper::idCount = 0;

std::map<ImguiWrapper::Mode, std::function<void(ImguiWrapper::Setting&)>> ImguiWrapper::funcs = { {ImguiWrapper::Mode::All, AllMode},{ImguiWrapper::Mode::Scale, ScaleMode},{ImguiWrapper::Mode::Rotate, RotateMode},{ImguiWrapper::Mode::Pos, PosMode} };

