/**
* @file ImguiWrapper.h
* @brief Imguiの静的クラス
* @author shiihara_kiyosumi
* @date 2022_12_20
*/
#pragma once
#include <functional>
#include <map>
#include "imgui.h"
#include "System/File.h"
#include "System/Common.h"

/**
* @class ImguiWrapper
* @brief Imguiのまとめクラス
*/
template <typename T>
class ImguiWrapper
{
public:

	/**
	* @enum Mode
	* @brief Settingのパラメーターとして使用する
	* @details Imguiでどこを変更できるようにするか決める
	*/
	enum class Mode
	{
		All,
		Scale,
		Rotate,
		Pos
	};

	/**
	* @struct Setting
	*/
	class Setting
	{
	public:
		Setting()
		{
			instance = nullptr;
			windowName = "";
			fileName = "";
			mode = Mode::All;
			id = GetID();
			AddID();
			saveButtonCounter = 0;
			
		}

		void Init(T& instance, const char* windowName, const char* fileName, Mode mode)
		{
			this->instance = &instance;
			this->windowName = windowName;
			this->fileName = fileName;
			this->mode = mode;
		}

		/*int GetSaveButtonCounter() { return saveButtonCounter; }
		void AddSaveButtonCounter() { saveButtonCounter++; }
		void InitSaveButtonCounter() { saveButtonCounter = 0; }*/

		T* instance;
		const char* windowName;
		const char* fileName;
		Mode mode;
		int id;
		int saveButtonCounter;
	};

	static void Action(Setting& setting)
	{
		funcs[setting.mode](setting);
	}


private:
	/**
	* @fn ImguiWrapper
	* @brief 非公開コンストラクタ
	*/
	ImguiWrapper() { /*このコンストラクタが呼び出される可能性はないので実装を書くべきではない*/ }

	static int idCount;

	static std::map<Mode, std::function<void(ImguiWrapper::Setting&)>> funcs;

	/*template <typename T>
	static void AllMode(Setting<T>& setting);
	template <typename T>
	static void ScaleMode(Setting<T>& setting);
	template <typename T>
	static void RotateMode(Setting<T>& setting);
	template <typename T>
	static void PosMode(Setting<T>& setting);*/

	static void AllMode(Setting& setting)
	{
		using namespace ImGui;
		Begin(setting.windowName);
		auto scale = setting.instance->GetScale();
		auto rotate = setting.instance->GetRotate();
		auto pos = setting.instance->GetPos();
		DragFloat("scaleX", &scale.x, 0.005f, 0.0f, 10.0f);
		DragFloat("scaleY", &scale.y, 0.005f, 0.0f, 10.0f);
		DragFloat("rotateZ", &rotate.z, 0.005f, 0.0f, 360.0f * 3.14f / 180.0f);
		DragFloat("posX", &pos.x, 0.5f, 0.0f, WINDOW_WIDTH);
		DragFloat("posY", &pos.y, 0.5f, 0.0f, WINDOW_HEIGHT);
		setting.instance->SetScale(scale);
		setting.instance->SetRotate(rotate);
		setting.instance->SetPos(pos);

		if (Button("Save"))
		{
			File::FileData data(pos.x, pos.y, scale.x, scale.y, rotate.z);
			File::OutputFile(setting.fileName, data);
			setting.saveButtonCounter++;
		}
		if (setting.saveButtonCounter != 0)
		{
			SameLine(); Text("SaveComplete!!");
			setting.saveButtonCounter = 0;
		}

		End();
	}

	static void ScaleMode(Setting& setting)
	{

	}

	static void RotateMode(Setting& setting)
	{

	}

	static void PosMode(Setting& setting)
	{

	}


	static void AddID() { idCount++; }
	static int GetID() { return idCount; }

	
};

