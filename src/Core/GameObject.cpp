#include "Core/GameObject.h"
#include <SDL.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include "MeshRenderer.h"


GameObject::GameObject() : BaseObject(), m_Position(glm::vec3(0, 0, 0)), m_Rotation(glm::vec3(0, 0, 0)), m_Scale(glm::vec3(1.0f, 1.0f, 1.0f)), m_ModelMatrixIsDirty(true)
{
	//
}

GameObject::GameObject(glm::vec3 aPosition, glm::quat aRotation, glm::vec3 aScale, bool aIsEnabled, std::string aTag) : BaseObject(), m_Position(aPosition), m_Rotation(aRotation), m_Scale(aScale)
{
	//
}

GameObject::GameObject(glm::vec3 aPosition, glm::vec3 aEulerAngles, glm::vec3 aScale, bool aIsEnabled, std::string aTag) : BaseObject(), m_Position(aPosition), m_Rotation(glm::quat(aEulerAngles)), m_Scale(aScale)
{
	//
}

GameObject::~GameObject()
{
	//
}


bool GameObject::Initialize()
{
	m_Initialized = true;

	//TODO: Should this be in it's own function?
	m_ModelMatrix = glm::translate(m_Position) * mat4_cast(m_Rotation) * glm::scale(m_Scale);
	m_ModelMatrixIsDirty = false;

	return true;
}


void GameObject::Update(double aDelta)
{
	//
}


void GameObject::SetPosition(glm::vec3 aPosition)
{
	m_Position = aPosition;
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetPosition(btVector3 &aPosition)
{
	m_Position = glm::vec3(aPosition.getX(), aPosition.getY(), aPosition.getZ());
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetPositionX(float aPositionX)
{
	m_Position.x = aPositionX;
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetPositionY(float aPositionY)
{
	m_Position.y = aPositionY;
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetPositionZ(float aPositionZ)
{
	m_Position.z = aPositionZ;
	m_ModelMatrixIsDirty = true;
}

glm::vec3 GameObject::GetPosition()
{
	return m_Position;
}

btVector3 GameObject::GetBTPosition()
{
	return btVector3(m_Position.x, m_Position.y, m_Position.z);
}


void GameObject::SetRotation(glm::quat aRotation)
{
	m_Rotation = aRotation;
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetRotation(glm::vec3 aEulerAngles)
{
	m_Rotation = glm::quat(aEulerAngles);
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetRotation(btQuaternion &aRotation)
{
	m_Rotation = glm::quat(aRotation.getX(), aRotation.getY(), aRotation.getZ(), aRotation.getW());
	m_ModelMatrixIsDirty = true;
}

void GameObject::Rotate(float aAngle, glm::vec3 aAxis)
{
	m_Rotation = glm::rotate(m_Rotation, aAngle, aAxis);
	m_ModelMatrixIsDirty = true;
}

glm::quat GameObject::GetRotation()
{
	return m_Rotation;
}

btQuaternion GameObject::GetBTRotation()
{
	return btQuaternion(m_Rotation.x, m_Rotation.y, m_Rotation.z, m_Rotation.w);
}


void GameObject::SetScale(glm::vec3 aScale)
{
	m_Scale = aScale;
}

void GameObject::SetScale(btVector3 &aScale)
{
	m_Scale = glm::vec3(aScale.getX(), aScale.getY(), aScale.getZ());
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetScaleX(float aScaleX)
{
	m_Scale.x = aScaleX;
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetScaleY(float aScaleY)
{
	m_Scale.y = aScaleY;
	m_ModelMatrixIsDirty = true;
}

void GameObject::SetScaleZ(float aScaleZ)
{
	m_Scale.z = aScaleZ;
	m_ModelMatrixIsDirty = true;
}

glm::vec3 GameObject::GetScale()
{
	return m_Scale;
}

btVector3 GameObject::GetBTScale()
{
	return btVector3(m_Scale.x, m_Scale.y, m_Scale.z);
}


glm::mat4x4 GameObject::GetModelMatrix()
{
	if (m_ModelMatrixIsDirty)
	{
		m_ModelMatrix = glm::translate(m_Position) * mat4_cast(m_Rotation) * glm::scale(m_Scale); // Scale, then Rotate, then Translate
		m_ModelMatrixIsDirty = false;
	}

	return m_ModelMatrix;
}



//template<class T> std::shared_ptr<T> GameObject::AddComponent()
//{
//	std::shared_ptr<T> component = std::make_shared<T>();
//
//	if (component->Initialize())
//	{
//		m_Components.push_back(component);
//
//		return component;
//	}
//	else
//	{
//		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Component could not initialize");
//
//		return nullptr;
//	}
//}

//void GameObject::AddComponent(std::shared_ptr<Component> aComponent)
//{
//	aComponent->AttachToGameObject(this);
//
//	if (aComponent->Initialize())
//	{
//		m_Components.push_back(aComponent);
//	}
//	else
//	{
//		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Component could not initialize");
//	}
//}

