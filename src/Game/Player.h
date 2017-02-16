#ifndef PLAYER_H
#define PLAYER_H

#include "Core/GameObject.h"
#include "Core/Mesh.h"

/* An enum representing the different states the player
can be in, such as when using powerups*/
enum PlayerStates
{
	PlayerShotReady, // The player is currently preparing to shoot
	PlayerNormal // The player is in motion, with no powerups active
};

class RigidBody;

class Player : public GameObject, Mesh
{
	glm::vec3 m_Velocity;

	float m_ForwardSpin;

	float m_SideSpin;

	bool m_IsIdle;

public:
	Player();
	virtual ~Player();

	virtual void Update(double delta);
	//virtual void Draw();

	// Sets the VBO and IBO for rendering this object based on the texture's dimensions
	virtual void SetupArrayBuffers();


	void SetVelocity(glm::vec3 velocity);
	glm::vec3 GetVelocity();

	bool IsIdle();

	void HandlePlatformCollision(GameObject* platform);

	void UpdateRBPos(RigidBody* rb);
};



#endif PLAYER_H