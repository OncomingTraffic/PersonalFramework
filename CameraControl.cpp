#include "CameraControl.h"
#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include "BaseObject.h"
#include "src\Core\Camera.h"

CameraControl::CameraControl(Camera* aCamera) : Behaviour(), m_Camera(aCamera), m_Pitch(0), m_Yaw(0), m_LockMouse(false), m_LockMouseX(0), m_LockMouseY(0)
{
	//
}

CameraControl::~CameraControl()
{
	//
}


void CameraControl::Cleanup()
{
	Behaviour::Cleanup();

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Camera Control cleanup");
}


void CameraControl::Update(float aDelta)
{
	HandleRotation(aDelta);

	const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

	int speedModifier =  1 + (int)KeyStates[SDL_SCANCODE_LSHIFT];

	// Test zooming, uses Field of View
	//if (KeyStates[SDL_SCANCODE_UP])
	//{
	//	m_Camera->SetZoom(m_Camera->GetZoom() + 1 * aDelta);
	//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Zoom: %f\n", m_Camera->GetZoom());
	//}
	//if (KeyStates[SDL_SCANCODE_DOWN])
	//{
	//	m_Camera->SetZoom(m_Camera->GetZoom() - 1 * aDelta);
	//	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Zoom: %f\n", m_Camera->GetZoom());
	//}

	if (KeyStates[SDL_SCANCODE_E])
	{
		MoveUp(5 * aDelta * speedModifier);
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Y Pos: %f\n", m_Parent->GetPosition().y);
	}
	if (KeyStates[SDL_SCANCODE_Q])
	{
		MoveUp(-5 * aDelta * speedModifier);
		//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Y Pos: %f\n", m_Parent->GetPosition().y);
	}

	if (KeyStates[SDL_SCANCODE_W])
	{
		MoveForward(5 * aDelta * speedModifier);
	}
	if (KeyStates[SDL_SCANCODE_S])
	{
		MoveForward(5 * -aDelta * speedModifier);
	}
	if (KeyStates[SDL_SCANCODE_A])
	{
		MoveRight(5 * -aDelta * speedModifier);
	}
	if (KeyStates[SDL_SCANCODE_D])
	{
		MoveRight(5 * aDelta * speedModifier);
	}
}


void CameraControl::MoveForward(float aDistance)
{
	//m_Parent->SetPosition(m_Parent->GetPosition() + m_ForwardVector * aDistance);
	//m_ViewProjIsDirty = true;
	m_Camera->MoveForward(aDistance);
}

void CameraControl::MoveUp(float aDistance)
{
	m_Camera->MoveUp(aDistance);
}

void CameraControl::MoveRight(float aDistance)
{
	m_Camera->MoveRight(aDistance);
}

void CameraControl::HandleRotation(float aDelta)
{
	int relX, relY;

	const float speed = 0.5f;

	if (SDL_GetRelativeMouseState(&relX, &relY) & SDL_BUTTON(3))
	{
		if (!m_LockMouse)
		{
			m_LockMouse = true;
			SDL_GetMouseState(&m_LockMouseX, &m_LockMouseY);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}

		if (abs(relX) > 0)
		{
			m_Yaw += relX * speed;

			glm::quat rot(glm::vec3(glm::radians(m_Pitch), glm::radians(m_Yaw), 0));
			m_Parent->SetRotation(rot);

			//m_Parent->Rotate(((float)relX) * aDelta, glm::vec3(0, 1, 0));

			//m_Camera->CalculateDirectionVectors();

		}
		if (abs(relY) > 0)
		{
			//m_Parent->Rotate(((float)relY) * aDelta, m_Camera->m_RightVector);

			m_Pitch += relY * speed;

			glm::quat rot(glm::vec3(glm::radians(m_Pitch), glm::radians(m_Yaw), 0));
			m_Parent->SetRotation(rot);

			//m_Camera->CalculateDirectionVectors();
		}
	}
	else
	{
		if (m_LockMouse)
		{
			m_LockMouse = false;
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}

	return;

	/*Old
	if (SDL_GetRelativeMouseState(&relX, &relY) & SDL_BUTTON(3))
	{
		if (abs(relX) > 2)
		{
			m_Parent->Rotate(((float)relX) * aDelta, glm::vec3(0, 1, 0));
			//glm::quat rot = glm::angleAxis(relX * aDelta, m_Camera->m_UpVector);
			//
			//m_Parent->SetRotation(m_Parent->GetRotation() * rot);

			//m_Parent->Rotate(((float)relX) * aDelta, m_Camera->m_UpVector);

			m_Camera->CalculateDirectionVectors();

			//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Relative Mouse state: %i, %i", relX, relY);
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Forward: %f, %f, %f", m_Camera->m_ForwardVector.x, m_Camera->m_ForwardVector.y, m_Camera->m_ForwardVector.z);
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Up: %f, %f, %f", m_Camera->m_UpVector.x, m_Camera->m_UpVector.y, m_Camera->m_UpVector.z);
			SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Right: %f, %f, %f", m_Camera->m_RightVector.x, m_Camera->m_RightVector.y, m_Camera->m_RightVector.z);
		}
		else if (abs(relY) > 2)
		{
			m_Parent->Rotate(((float)relY) * aDelta, m_Camera->m_RightVector);
			//glm::quat rot = glm::angleAxis(relY * aDelta, m_Camera->m_RightVector);
			//
			//m_Parent->SetRotation(m_Parent->GetRotation() * rot);

			m_Camera->CalculateDirectionVectors();
		}
	}
	*/
}

std::string CameraControl::GetType()
{
	return std::string("CameraControl");
}