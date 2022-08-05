#pragma once
#include <vector>
#include "CameraStruct.h"

using std::vector;



class TellCameraData
{
public:

	static void CreateInstance()
	{
		if (!m_this)
		{
			m_this = new TellCameraData;
		}
	}

	static void DeleteInstance()
	{
		if (m_this)
		{
			delete m_this;
			m_this = nullptr;
		}
	}

	static void AddCamera(const CameraData& data)
	{
		m_this->m_cameraDataVec.emplace_back(data);
	}

	static bool SubCamera(const CameraData& data)
	{
		auto& cameraDataVec = m_this->m_cameraDataVec;

		if (cameraDataVec.empty())
		{
			return false;
		}

		for (auto it = cameraDataVec.begin(); it != cameraDataVec.end();)
		{
			if (it->key == data.key)
			{
				it = cameraDataVec.erase(it);
				return true;
			}
			it++;
		}
		return false;
	}

	static bool GetCameraData(CameraData* cameraData, CAMERA_NUMBER key)
	{
		auto& cameraDataVec = m_this->m_cameraDataVec;
		for (auto it = cameraDataVec.begin(); it != cameraDataVec.end(); it++)
		{
			if (it->key == key)
			{
				*cameraData = *it;
				return true;
			}
		}
		return false;
	}

private:
	TellCameraData();
	~TellCameraData();

	vector<CameraData> m_cameraDataVec;

	static TellCameraData* m_this;

};