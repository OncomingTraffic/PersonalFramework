#include "GameWindow.h"

#include <GL\glew.h>
#include <SDL_opengl.h>
#include <gl\GLU.h>
//#include "ShaderProgram.h"
#include <SDL_image.h>

GameWindow::GameWindow() : m_GameWindow(nullptr)
{
	//
}

GameWindow::~GameWindow()
{
	Close();
}

bool GameWindow::Initialize(const char* aWindowTitle, int aWindowWidth, int aWindowHeight, int aWindowPosX, int aWindowPosY, Uint32 aWindowFlags)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create window
		m_GameWindow = SDL_CreateWindow(aWindowTitle, aWindowPosX, aWindowPosY, aWindowWidth, aWindowHeight, aWindowFlags);
		if (m_GameWindow == NULL)
		{
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create context
			m_Context = SDL_GL_CreateContext(m_GameWindow);
			if (m_Context == NULL)
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Error initializing GLEW! %s\n", glewGetErrorString(glewError));
				}

				//Use Vsync
				if (SDL_GL_SetSwapInterval(1) < 0)
				{
					SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Unable to set VSync! SDL Error: %s\n", SDL_GetError());
				}

				glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
			}
		}
	}

	return success;
}

void GameWindow::Close()
{
	//Destroy window	
	SDL_DestroyWindow(m_GameWindow);
	m_GameWindow = nullptr;

	//Quit SDL subsystems
	if (IMG_Init(0))
	{
		IMG_Quit();
	}

	// Quit SDL
	SDL_Quit();

	/*
	// Free the renderer from memory
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}

	// Free the window from memory
	if (m_GameWindow != nullptr)
	{
		SDL_DestroyWindow(m_GameWindow);
		m_GameWindow = nullptr;
	}

	// If the SDL_image library is loaded, close it
	if (IMG_Init(0))
	{
		IMG_Quit();
	}

	// Close all audio channels
	while (Mix_QuerySpec(nullptr, nullptr, nullptr))
	{
		Mix_CloseAudio();
	}

	// Close all open mixers
	while (Mix_Init(0))
	{
		Mix_Quit();
	}
	//TTF_Quit();

	// Close SDL
	SDL_Quit();
	*/
}

SDL_Window* GameWindow::GetWindow()
{
	return m_GameWindow;
}