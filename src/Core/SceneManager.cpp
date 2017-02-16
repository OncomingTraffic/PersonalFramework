#include "SceneManager.h"
#include "Scene.h"
#include <SDL.h>

SceneManager::SceneManager() : m_ActiveScene(nullptr)
{
	//
}

SceneManager::~SceneManager()
{
	for (unsigned int i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes.at(i) != nullptr)
		{
			delete m_Scenes.at(i);
		}
	}

	m_Scenes.clear();
}


void SceneManager::Update(float aDelta)
{
	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->Update(aDelta);
	}
}


void SceneManager::Draw()
{
	if (m_ActiveScene != nullptr)
	{
		m_ActiveScene->Draw();
	}
}


void SceneManager::AddScene(Scene* aScene, bool aMakeActive)
{
	m_Scenes.push_back(aScene);

	if (aMakeActive)
	{
		m_ActiveScene = aScene;
		if (!aScene->IsLoaded())
		{
			if (!LoadScene(aScene))
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Scene was unable to load: %s\n", aScene->GetTag());
			}
		}
		m_ActiveScene = aScene;
	}
}

void SceneManager::RemoveScene(std::string aSceneTag, std::string aNewSceneTag)
{
	for (unsigned int i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes.at(i)->GetTag() == aSceneTag)
		{
			if (m_Scenes.at(i) == m_ActiveScene)
			{
				m_ActiveScene = nullptr;
			}
			if (m_Scenes.at(i) != nullptr)
			{
				delete m_Scenes.at(i);
			}
			m_Scenes.erase(m_Scenes.begin() + i);
		}
	}
}

void SceneManager::SetActiveScene(std::string aSceneTag)
{
	for (unsigned int i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes.at(i)->GetTag() == aSceneTag)
		{
			if (!m_Scenes.at(i)->IsLoaded())
			{
				if (!LoadScene(m_Scenes.at(i)))
				{
					SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Scene was unable to load: %s\n", aSceneTag);
					break;
				}
			}
			m_ActiveScene = m_Scenes.at(i);
			break;
		}
	}
}

void SceneManager::RemoveInactiveScenes()
{
	for (unsigned int i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes.at(i) != m_ActiveScene)
		{
			if (m_Scenes.at(i) != nullptr)
			{
				m_Scenes.at(i)->Cleanup();
				delete m_Scenes.at(i);
			}
		}
	}

	m_Scenes.clear();
	m_Scenes.push_back(m_ActiveScene);
}

//TODO: Maybe change this to a void function if return is unnecessary
bool SceneManager::LoadScene(Scene* aScene)
{
	float LoadPercentage = 0.0f;

	while (LoadPercentage < 1.0f)
	{
		LoadPercentage = aScene->Load();
	}

	return true;
}

Scene* SceneManager::GetActiveScene()
{
	SDL_assert(m_ActiveScene != nullptr);
	return m_ActiveScene;
}