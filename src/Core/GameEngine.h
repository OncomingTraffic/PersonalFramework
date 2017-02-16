#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <memory>

class GameWindow;
class GameTimer;
class SceneManager;


class GameEngine
{
	// The window used to render
	GameWindow* m_GameWindow;

	// A timer used to limit frame rate and get delta time
	GameTimer* m_GameTimer;

	// The exit condition for the game loop
	bool m_Exit;

	// The time passed between iterations of the game loop
	//TODO: Should this be a double or float?
	float m_DeltaTime;

	//
	SDL_Event m_Event;

	//
	SceneManager* m_SceneManager;

public:
	GameEngine();
	~GameEngine();

	bool Load();

	void GameLoop();

	void Update();
	void Draw();
};


#endif GAME_H