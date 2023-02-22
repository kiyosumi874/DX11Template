/**
* @file ImguiWrapper.h
* @brief Imguiの静的クラス template static mapの初期化バグを後回しにしています
* @author shiihara_kiyosumi
* @date 2022_12_20
*/

#include "ImguiWrapper.h"




// 静的メンバ変数の定義
int ImguiWrapper::idCount = 0;

template <typename T>
std::map<ImguiWrapper::Mode, std::function<void(ImguiWrapper::Setting<T>&)>> ImguiWrapper::funcs = { {ImguiWrapper::Mode::All, AllMode},{ImguiWrapper::Mode::Scale, ScaleMode},{ImguiWrapper::Mode::Rotate, RotateMode},{ImguiWrapper::Mode::Pos, PosMode} };


//template <typename T>
//void ImguiWrapper::AllMode(Setting<T>& setting)
//{
//	Begin(setting.windowName);
//	auto scale = setting.instance->GetScale();
//	auto rotate = setting.instance->GetRotate();
//	auto pos = setting.instance->GetPos();
//	DragFloat("scaleX", &scale.x, 0.005f, 0.0f, 10.0f);
//	DragFloat("scaleY", &scale.y, 0.005f, 0.0f, 10.0f);
//	DragFloat("rotateZ", &rotate.z, 0.005f, 0.0f, 360.0f * 3.14f / 180.0f);
//	DragFloat("posX", &pos.x, 0.5f, 0.0f, WINDOW_WIDTH);
//	DragFloat("posY", &pos.y, 0.5f, 0.0f, WINDOW_HEIGHT);
//	setting.instance->SetScale(scale);
//	setting.instance->SetRotate(rotate);
//	setting.instance->SetPos(pos);
//
//	if (Button("Save"))
//	{
//		File::FileData data(pos.x, pos.y, scale.x, scale.y, rotate.z);
//		File::OutputFile(setting.fileName, data);
//		setting.saveButtonCounter++;
//	}
//	if (setting.saveButtonCounter != 0)
//	{
//		SameLine(); Text("SaveComplete!!");
//		setting.saveButtonCounter = 0;
//	}
//
//	End();
//}

//template <typename T>
//void ImguiWrapper::ScaleMode(Setting<T>& setting)
//{
//}
//
//template <typename T>
//void ImguiWrapper::RotateMode(Setting<T>& setting)
//{
//}
//
//template <typename T>
//void ImguiWrapper::PosMode(Setting<T>& setting)
//{
//}
