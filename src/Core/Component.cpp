#include "Component.h"
#include "../BaseObject.h"
#include <SDL.h>



Component::Component() : m_Parent(nullptr)
{
	//
}

Component::~Component()
{
	m_Parent = nullptr;
}


//bool Component::Initialize()
//{
//	return true;
//}

void Component::AttachToGameObject(BaseObject* aObject)
{
	m_Parent = aObject;
}

BaseObject* Component::GetParent()
{
	return m_Parent;
}

void Component::Cleanup()
{
	//
}