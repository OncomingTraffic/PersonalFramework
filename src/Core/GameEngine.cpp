#include "GameEngine.h"

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
#include "Core/GameTimer.h"
#include "Core/GameServices.h"
#include "Core/SceneManager.h"
#include "Core/GameWindow.h"
#include "Core/Scene.h"
#include "Core/Camera.h"
#include "Game/TestScene.h"


GameEngine::GameEngine() : m_Exit(false), m_DeltaTime(0.0), m_GameTimer(nullptr), m_SceneManager(nullptr)
{
	//
}

GameEngine::~GameEngine()
{
	if (m_GameTimer != nullptr)
	{
		delete m_GameTimer;
		m_GameTimer = nullptr;
	}

	GameServices::RemoveAll();
}


bool GameEngine::Load()
{
	m_GameWindow = new GameWindow();

	// TODO: Add a way to change window sizes
	// TODO: Add startup preferences for window, audio, etc.
	if (!m_GameWindow->Initialize("Main WIndow", 640, 480, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL))
	{
		return false;
	}
	GameServices::SetGameWindow(m_GameWindow);

	m_GameTimer = new GameTimer(60.0f);
	GameServices::LoadAll();

	m_SceneManager = GameServices::GetSceneManager();
	m_SceneManager->AddScene(new TestScene(), true);
	

	return true;
}

void GameEngine::GameLoop()
{
	while (!m_Exit)
	{
		// TODO: See if there is a better way to handle events, especially with event dispatching
		while (SDL_PollEvent(&m_Event) != 0)
		{
			if (m_Event.type == SDL_QUIT)
			{
				m_Exit = true;
			}
		}
		m_GameTimer->StartFrameTimer();

		Update();

		Draw();

		// TODO: Add a boolean to control whether to cap the frame rate or not
		m_DeltaTime = m_GameTimer->LimitFrameRate();
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Current Delta Time: %f\n", m_DeltaTime);

	}
}

void GameEngine::Update()
{
	m_SceneManager->Update(m_DeltaTime);
}

void GameEngine::Draw()
{
	// TODO: Move clear flags to the active camera
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_SceneManager->Draw();

	// TODO: See if SwapWindow should be called within the GameWindow class instead
	SDL_GL_SwapWindow(m_GameWindow->GetWindow());
}