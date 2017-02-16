#include "PlayerControl.h"
#include "src\Core\PhysicsObject.h"
#include <SDL.h>



PlayerControl::PlayerControl() : Behaviour(), m_PhysicsObject(nullptr)
{

}

PlayerControl::~PlayerControl()
{

}

bool PlayerControl::Initialize()
{
	if (!Behaviour::Initialize())
	{
		return false;
	}

	m_PhysicsObject = dynamic_cast<PhysicsObject*>(m_Parent);

	if (m_PhysicsObject == nullptr)
	{
		return false;
	}

	return true;
}

void PlayerControl::Cleanup()
{
	Behaviour::Cleanup();
}


void PlayerControl::Update(float aDelta)
{
	if (m_PhysicsObject->GetRigidbody()->getActivationState() == ISLAND_SLEEPING)
	{
		const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

		if (KeyStates[SDL_SCANCODE_UP])
		{
			m_PhysicsObject->GetRigidbody()->activate();
			m_PhysicsObject->GetRigidbody()->applyCentralImpulse(btVector3(0, 10, 5));
		}
		else if (KeyStates[SDL_SCANCODE_RIGHT])
		{
			m_PhysicsObject->GetRigidbody()->activate();
			//m_PhysicsObject->GetRigidbody()->applyCentralImpulse(btVector3(5, 10, 0));
			m_PhysicsObject->GetRigidbody()->applyTorqueImpulse(btVector3(100, 0, 0));
		}
		else if (KeyStates[SDL_SCANCODE_DOWN])
		{
			m_PhysicsObject->GetRigidbody()->activate();
			m_PhysicsObject->GetRigidbody()->applyCentralImpulse(btVector3(0, 10, -5));
		}
		else if (KeyStates[SDL_SCANCODE_LEFT])
		{
			m_PhysicsObject->GetRigidbody()->activate();
			m_PhysicsObject->GetRigidbody()->applyCentralImpulse(btVector3(-5, 10, 0));
		}
	}
}


std::string PlayerControl::GetType()
{
	return std::string("PlayerControl");
}