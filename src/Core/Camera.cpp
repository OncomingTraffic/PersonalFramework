#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\vector_angle.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\orthonormalize.hpp>
#include <SDL.h>
#include "../BaseObject.h"
#include "Scene.h"
#include "../RenderingManager.h"

Camera::Camera() : Component(), m_Tag("Default"), m_Zoom(1.0f), m_ViewProjIsDirty(false)
{
	//
}

Camera::Camera(std::string aTag, float aZoom) : m_Tag(aTag), m_Zoom(aZoom), m_ViewProjIsDirty(false)
{
	//
}

Camera::~Camera()
{
	//
}


bool Camera::Initialize()
{
	SDL_assert(m_Parent != nullptr);
	SDL_assert(m_Parent->GetParentScene() != nullptr);

	if (m_Parent->GetParentScene() != nullptr)
	{
		m_Parent->GetParentScene()->GetRenderingManager()->AddCamera(this, true);

		CalculateViewProjectionMatrix();

		return true;
	}

	return false;
}

void Camera::Cleanup()
{
	if (m_Parent->GetParentScene() != nullptr)
	{
		m_Parent->GetParentScene()->GetRenderingManager()->RemoveCamera(m_Tag);

		CalculateViewProjectionMatrix();
	}
}


void Camera::Update(float aDelta)
{
	return;

	const Uint8* KeyStates = SDL_GetKeyboardState(NULL);

	// Test zooming, uses Field of View
	if (KeyStates[SDL_SCANCODE_UP])
	{
		m_Zoom += 1 * aDelta;
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Zoom: %f\n", m_Zoom);
	}
	if (KeyStates[SDL_SCANCODE_DOWN])
	{
		m_Zoom -= 1 * aDelta;
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Zoom: %f\n", m_Zoom);
	}

	if (KeyStates[SDL_SCANCODE_E])
	{
		MoveUp(5 * aDelta);
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Y Pos: %f\n", m_Parent->GetPosition().y);
	}
	if (KeyStates[SDL_SCANCODE_Q])
	{
		MoveUp(-5 * aDelta);
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Y Pos: %f\n", m_Parent->GetPosition().y);
	}

	if (KeyStates[SDL_SCANCODE_W])
	{
		MoveForward(5 * aDelta);
	}
	if (KeyStates[SDL_SCANCODE_S])
	{
		MoveForward(5 * -aDelta);
	}
	if (KeyStates[SDL_SCANCODE_A])
	{
		MoveRight(5 * -aDelta);
	}
	if (KeyStates[SDL_SCANCODE_D])
	{
		MoveRight(5 * aDelta);
	}
}

void Camera::MoveForward(float distance)
{
	m_Parent->SetPosition(m_Parent->GetPosition() + m_ForwardVector * distance);
	m_ViewProjIsDirty = true;
}

void Camera::MoveUp(float distance)
{
	m_Parent->SetPosition(m_Parent->GetPosition() + m_UpVector * distance);
	m_ViewProjIsDirty = true;
}

void Camera::MoveRight(float distance)
{
	m_Parent->SetPosition(m_Parent->GetPosition() + m_RightVector * distance);
	m_ViewProjIsDirty = true;
}

void Camera::SetZoom(float aZoom)
{
	m_Zoom = aZoom;
	m_ViewProjIsDirty = true;
}

float Camera::GetZoom()
{
	return m_Zoom;
}

void Camera::LookAtTarget(glm::vec3 aPosition)
{

	// Calculate a rotation quaternion to align the camera to the direction vector
	glm::quat rot(glm::lookAtLH(m_Parent->GetPosition(), aPosition, glm::vec3(0, 1, 0))); // This always keeps the camera aligned upwards, TODO: Find a way to mantain roll orientation

	// Inverse the rotation, since lookAt actually rotates the world to align with the camera
	m_Parent->SetRotation(glm::inverse(rot));

	// ViewProjectionMatrix needs to be recalculated
	m_ViewProjIsDirty = true;
}


void Camera::CalculateViewProjectionMatrix()
{
	CalculateDirectionVectors();
	//CameraUp = CameraMatrix * vec4(0,1,0,0)
	//CameraRight = CameraMatrix * vec4(1, 0, 0, 0)
	//CameraForward = CameraMatrix * vec4(0, 0, -1, 0)

	// Obtain the forward vector by multiplying the rotation quaternion by a standard forward vector (vec4(0, 0, 1),
	// then add it to the cameras postion. This will result in a position one unit "forward" from the camera
	glm::mat4 view = glm::lookAtLH(m_Parent->GetPosition(), m_ForwardVector + m_Parent->GetPosition(), m_UpVector); // "center" seems to represent a vector that goes from the camera's postion ("eye") to the location
	//glm::mat4 view = glm::lookAt(m_Position,glm::vec3(0, 0, 0), glm::vec3(0, 1, 1));

	m_ViewProjMatrix = glm::perspectiveLH(glm::radians(60.f), 640.0f / 480.0f, 0.01f, 1000.0f) * view; // Zoom increases the FOV, causing things to appear smaller
	
	// TODO: Find a good value for zoom amount. Zoom seems to be inverse - higher zoom means things are farther away. FOV is in radians

	m_ViewProjIsDirty = false;

	m_ViewFrustum.CreateFromMatrix(glm::transpose(m_ViewProjMatrix));
}

void Camera::CalculateDirectionVectors()
{
	glm::mat4 matrix = m_Parent->GetModelMatrix();

	m_ForwardVector = glm::vec3(matrix[2][0], matrix[2][1], matrix[2][2]);
	m_UpVector = glm::vec3(matrix[1][0], matrix[1][1], matrix[1][2]);
	m_RightVector = glm::vec3(matrix[0][0], matrix[0][1], matrix[0][2]);

	return;

	// The default forward vector is assumed to be (0, 0, 1)
	m_ForwardVector = glm::vec3(glm::normalize((m_Parent->GetRotation()) * glm::vec3(0, 0, 1)));

	// The default up vector is assumed to be (0, 1, 0)
	m_UpVector = glm::vec3(glm::normalize((m_Parent->GetRotation()) * glm::vec3(0, 1, 0)));

	// Calculate the right vector, which is perpendicular to both the up and forward vectors
	//TODO: Double check this. Swapped the two arguments so that right is the correct direction (was negative before)
	m_RightVector = glm::cross(m_UpVector, m_ForwardVector);

	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Forward (%f, %f, %f)\n", m_ForwardVector.x, m_ForwardVector.y, m_ForwardVector.z);
	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Up (%f, %f, %f)\n", m_UpVector.x, m_UpVector.y, m_UpVector.z);
	//SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Right (%f, %f, %f)\n", m_RightVector.x, m_RightVector.y, m_RightVector.z);

	/* This seems to do the same thing
	glm::mat4 mat = glm::toMat4(rotQuat);

	m_ForwardVector = glm::vec3(glm::normalize(glm::inverse(mat) * glm::vec4(0, 0, -1, 0)));
	*/
}

glm::mat4 Camera::GetViewProjectionMatrix()
{
	//if (m_ViewProjIsDirty)
	{
		CalculateViewProjectionMatrix();
	}

	return m_ViewProjMatrix;
}

Frustum Camera::GetViewFrustum()
{
	return m_ViewFrustum;
}

std::string Camera::GetTag()
{
	return m_Tag;
}

std::string Camera::GetType()
{
	return std::string("Camera");
}