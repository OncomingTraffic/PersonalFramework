#include "GameServices.h"
#include <memory>
#include "../ShaderDefault.h"



AudioManager* GameServices::m_AudioManager = nullptr;
FontManager* GameServices::m_FontManager = nullptr;
ShaderManager* GameServices::m_ShaderManager = nullptr;
SceneManager* GameServices::m_SceneManager = nullptr;
GameWindow* GameServices::m_GameWindow = nullptr;
std::shared_ptr<ResourceManager> GameServices::m_ResourceManager = nullptr;

AudioManager* GameServices::GetAudioManager()
{
	return m_AudioManager;
}

FontManager* GameServices::GetFontManager()
{
	return m_FontManager;
}

ShaderManager* GameServices::GetShaderManager()
{
	return m_ShaderManager;
}

SceneManager* GameServices::GetSceneManager()
{
	return m_SceneManager;
}

GameWindow* GameServices::GetGameWindow()
{
	return m_GameWindow;
}

std::shared_ptr<ResourceManager> GameServices::GetResourceManager()
{
	return m_ResourceManager;
}


void GameServices::SetAudioManager(AudioManager* aAudioManager)
{
	m_AudioManager = aAudioManager;
}

void GameServices::SetFontManager(FontManager* aFontManager)
{
	m_FontManager = aFontManager;
}

void GameServices::SetShaderManager(ShaderManager* aShaderManager)
{
	m_ShaderManager = aShaderManager;
}

void GameServices::SetSceneManager(SceneManager* aSceneManager)
{
	m_SceneManager = aSceneManager;
}

void GameServices::SetGameWindow(GameWindow* aGameWindow)
{
	m_GameWindow = aGameWindow;
}

//TODO: Change the return type if necessary
bool GameServices::LoadAll()
{
	if (m_FontManager == nullptr)
	{
		m_FontManager = new FontManager();
	}
	if (m_ShaderManager == nullptr)
	{
		m_ShaderManager = new ShaderManager();
	}
	if (m_AudioManager == nullptr)
	{
		m_AudioManager = new AudioManager();
	}
	if (m_SceneManager == nullptr)
	{
		m_SceneManager = new SceneManager();
	}

	//TODO: Move this elsewhere
	m_ShaderManager->AddShader("Default", std::make_shared<ShaderDefault>());

	m_ResourceManager = std::make_shared<ResourceManager>();

	if (!m_ResourceManager->Initialize())
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "ResourceManager could not initialize!");
		return false;
	}

	return true;
}

void GameServices::RemoveAll()
{
	if (m_FontManager != nullptr)
	{
		delete m_FontManager;
		m_FontManager = nullptr;
	}

	if (m_SceneManager != nullptr)
	{
		delete m_SceneManager;
		m_SceneManager = nullptr;
	}

	if (m_AudioManager != nullptr)
	{
		delete m_AudioManager;
		m_AudioManager = nullptr;
	}

	if (m_ShaderManager != nullptr)
	{
		delete m_ShaderManager;
		m_ShaderManager = nullptr;
	}

	if (m_GameWindow != nullptr)
	{
		delete m_GameWindow;
		m_GameWindow = nullptr;
	}
	
}