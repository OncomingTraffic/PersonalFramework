#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Core/Scene.h"
#include <SDL_mixer.h>
#include <memory>
#include "../src/Core/GameObject.h"

class Sprite;
class Label;
class Cubes;
class Player;
class Platform;
class RigidBody;
class PhysicsObject;
class Camera;

class TestScene : public Scene
{
public:
	TestScene();
	~TestScene();

	virtual float Load();

	virtual void Cleanup();

	virtual void Update(float delta);
	virtual void Draw();

protected:

	bool m_Down;

	std::shared_ptr<Player> player;

	std::shared_ptr<PhysicsObject> platform;

	std::shared_ptr<PhysicsObject> sphere;

	std::unique_ptr<GameObject> cameraHolder;
	Camera* cam;

	RigidBody* rb;

	std::shared_ptr<Mix_Music> music;
	//Cubes* cubes;
	//Platform* platform;
	//Sprite* sprite;
	//Label* label;
	//Label* label2;



};


#endif TESTSCENE_H