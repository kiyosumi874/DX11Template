/**
* @file ImguiWrapper.h
* @brief Imguiの静的クラス
* @author shiihara_kiyosumi
* @date 2022_12_20
*/
#pragma once
#include <functional>
#include <map>
/**
* @class ImguiWrapper
* @brief Imguiのまとめクラス
*/
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
	template <typename T>
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
	private:
		int id;
		int saveButtonCounter;
	};

	template <typename T>
	static void Action(Setting<T>& setting)
	{
		funcs<T>[setting.mode](setting);
	}
	
private:
	/**
	* @fn ImguiWrapper
	* @brief 非公開コンストラクタ
	*/
	ImguiWrapper() { /*このコンストラクタが呼び出される可能性はないので実装を書くべきではない*/ }

	static int idCount;

	template <typename T>
	static std::map<Mode, std::function<void(ImguiWrapper::Setting<T>&)>> funcs;

	template <typename T>
	static void AllMode(Setting<T>& setting);
	template <typename T>
	static void ScaleMode(Setting<T>& setting);
	template <typename T>
	static void RotateMode(Setting<T>& setting);
	template <typename T>
	static void PosMode(Setting<T>& setting);

	static void AddID() { idCount++; }
	static int GetID() { return idCount; }

	
};

