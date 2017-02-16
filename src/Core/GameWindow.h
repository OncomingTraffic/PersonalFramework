#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <SDL.h>
#include <string>

//class ShaderProgram;

class GameWindow
{
	SDL_Window* m_GameWindow;

	// The OpenGL context
	SDL_GLContext m_Context;

public:
	GameWindow();
	~GameWindow();

	// Initialize SDL, the window, OpenGL, and SDL subsystems. Returns false if anything fails to initialize
	bool Initialize(const char* windowTitle = "Main Window", int windowWidth = 640, int windowHeight = 480, int windowPosX = SDL_WINDOWPOS_UNDEFINED,
		int windowPosY = SDL_WINDOWPOS_UNDEFINED, Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	
	void Close();

	SDL_Window* GetWindow();

	unsigned int GetWindowWidth();

	unsigned int GetWindowHeight();
};


#endif GAMEWINDOW_H