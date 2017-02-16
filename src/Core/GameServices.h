#ifndef GAMESERVICES_H
#define GAMESERVICES_H

#include <memory>

//TODO: Change these to forward declarations
#include "FontManager.h"
#include "ShaderManager.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "GameWindow.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Scene.h"


class GameServices
{
	static AudioManager* m_AudioManager;
	static FontManager* m_FontManager;
	static ShaderManager* m_ShaderManager;
	static SceneManager* m_SceneManager;
	static GameWindow* m_GameWindow; //TODO: Add the ability to have multiple windows
	static std::shared_ptr<ResourceManager> m_ResourceManager;

public:
	GameServices();
	~GameServices();

	static AudioManager* GetAudioManager();
	static FontManager* GetFontManager();
	static ShaderManager* GetShaderManager();
	static SceneManager* GetSceneManager();
	static GameWindow* GetGameWindow();
	static std::shared_ptr<ResourceManager> GetResourceManager();

	static void SetAudioManager(AudioManager* audioManager);
	static void SetFontManager(FontManager* fontManager);
	static void SetShaderManager(ShaderManager* shaderManager);
	static void SetSceneManager(SceneManager* sceneManager);
	static void SetGameWindow(GameWindow* gameWindow);

	static bool LoadAll();

	static void RemoveAll();
};

#endif GAMESERVICES_H