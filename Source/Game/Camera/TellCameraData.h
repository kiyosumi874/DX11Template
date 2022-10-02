/**
* @file TellCameraData.h
* @brief 複数のカメラの情報を伝える(シングルトン)
* @author shiihara_kiyosumi
* @date 2022_08_05
*/

// ヘッダーファイルのインクルード
#pragma once
#include <unordered_map>
#include "CameraStruct.h"
#include "System/Common.h"

// using宣言
using std::unordered_map;

/**
* @class TellCameraData
* @brief 複数のカメラの情報を伝えるクラス(シングルトン)
*/
class TellCameraData
{
public:
	/**
	* @fn CreateInstance
	* @brief 自分のインスタンスを生成
	*/
	static void CreateInstance()
	{
		if (!m_this)
		{
			m_this = new TellCameraData;
		}
	}

	/**
    * @fn DeleteInstance
	* @brief 自分のインスタンスを削除
	*/
	static void DeleteInstance()
	{
		if (m_this)
		{
			delete m_this;
			m_this = nullptr;
		}
	}

	/**
	* @fn AddCamera
	* @brief カメラの情報を追加
	* @param[in] data カメラの情報
	*/
	static void AddCamera(const CameraData& data)
	{
		m_this->m_cameraDataVec[data.key] = data;
	}

	/**
	* @fn SubCamera
	* @brief カメラの情報を削除
	* @param[in] data カメラの情報
	*/
	static bool SubCamera(const CAMERA_NUMBER key)
	{
		auto& cameraDataVec = m_this->m_cameraDataVec;

		if (cameraDataVec.empty())
		{
			MSG("Cameraは現在登録されていません")
			return false;
		}

		cameraDataVec.erase(key);
		return false;
	}

	/**
	* @fn GetCameraData
	* @brief カメラの情報を取得
	* @param[out] cameraData これにカメラのデータを吐き出す
	* @param[in] key 個々カメラのID
	* @return bool trueで成功
	*/
	static bool GetCameraData(CameraData* cameraData, CAMERA_NUMBER key)
	{
		auto& cameraDataVec = m_this->m_cameraDataVec;
		*cameraData = cameraDataVec[key];
		return false;
	}


private:
	/**
	* @fn Fps
	* @brief コンストラクタ
	*/
	TellCameraData();

	/**
	* @fn ~Fps
	* @brief デストラクタ
	*/
	~TellCameraData();

	unordered_map<CAMERA_NUMBER,CameraData> m_cameraDataVec; // カメラの情報を保存する配列

	static TellCameraData* m_this; // 静的な自分のクラスのポインタ

};