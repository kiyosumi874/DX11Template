/**
* @file ImguiWrapper.h
* @brief Imgui�̐ÓI�N���X
* @author shiihara_kiyosumi
* @date 2022_12_20
*/
#pragma once
#include <functional>
#include <map>
/**
* @class ImguiWrapper
* @brief Imgui�̂܂Ƃ߃N���X
*/
class ImguiWrapper
{
public:

	/**
	* @enum Mode
	* @brief Setting�̃p�����[�^�[�Ƃ��Ďg�p����
	* @details Imgui�łǂ���ύX�ł���悤�ɂ��邩���߂�
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
	* @brief ����J�R���X�g���N�^
	*/
	ImguiWrapper() { /*���̃R���X�g���N�^���Ăяo�����\���͂Ȃ��̂Ŏ����������ׂ��ł͂Ȃ�*/ }

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

