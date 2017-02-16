#include "CameraManager.h"
#include "Camera.h"


CameraManager::CameraManager() : m_ActiveCamera(nullptr)
{
	//
}

CameraManager::~CameraManager()
{
	for (int i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras.at(i) != nullptr)
		{
			delete m_Cameras.at(i);
			m_Cameras.at(i) = nullptr;
		}
	}
	m_Cameras.clear();
}


void CameraManager::AddCamera(Camera* aCamera, bool aMakeActive)
{
	if (aCamera != nullptr)
	{
		m_Cameras.push_back(aCamera);

		if (aMakeActive)
		{
			m_ActiveCamera = aCamera;
		}
	}


}

void CameraManager::RemoveCamera(std::string aCameraTag)
{
	for (int i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras.at(i)->GetTag() != aCameraTag)
		{
			if (m_Cameras.at(i) != nullptr)
			{
				delete m_Cameras.at(i);
				m_Cameras.at(i) = nullptr;
			}

			m_Cameras.erase(m_Cameras.begin() + i);
		}
	}
}

void CameraManager::SetActiveCamera(std::string aCameraTag)
{
	for (int i = 0; i < m_Cameras.size(); ++i)
	{
		if (m_Cameras.at(i)->GetTag() != aCameraTag)
		{
			m_ActiveCamera = m_Cameras.at(i);
			break;
		}
	}
}

Camera* CameraManager::GetActiveCamera()
{
	return m_ActiveCamera;
}