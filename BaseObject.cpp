#include "BaseObject.h"
#include <SDL.h>
#include "src\Core\Scene.h"
#include "src\Core\Component.h"


BaseObject::BaseObject() : m_ParentScene(nullptr), m_Initialized(false), m_Enabled(true)
{
	//
}

BaseObject::BaseObject(std::string aTag) : m_ParentScene(nullptr), m_Initialized(false), m_Enabled(true), m_Tag(aTag)
{
	//
}

BaseObject::~BaseObject()
{
	//
}


bool BaseObject::Initialize()
{
	m_Initialized = true;

	for (std::vector<std::shared_ptr<Component>>::iterator componentIter = m_Components.begin(); componentIter != m_Components.end(); componentIter++)
	{
		//TODO: Will a BaseObject ever initialize after components have been added?
	}

	return true;
}

void BaseObject::Cleanup()
{
	// Cleanup and remove all components from this object
	RemoveAllComponents();
	if (m_Components.size() > 0)
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Not all components were deleted");
	}

	//TODO: Should children be removed before components?
	while (m_Children.size() > 0)
	{
		m_Children.back()->Cleanup();
		m_Children.pop_back();
	}

	m_Initialized = false;
}


void BaseObject::SetTag(std::string aTag)
{
	m_Tag = aTag;
}

std::string BaseObject::GetTag()
{
	return m_Tag;
}


bool BaseObject::WasInitialized()
{
	return m_Initialized;
}


void BaseObject::AddComponent(std::shared_ptr<Component> aComponent)
{
	aComponent->AttachToGameObject(this);

	if (aComponent->Initialize())
	{
		m_Components.push_back(aComponent);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Component could not initialize");
	}
}

void BaseObject::RemoveComponent(std::shared_ptr<Component> aComponent)
{
	std::vector<std::shared_ptr<Component>>::iterator iter = std::find(m_Components.begin(), m_Components.end(), aComponent);

	if (iter != m_Components.end())
	{
		aComponent->Cleanup();
		m_Components.erase(iter);
	}
	else
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "Component could not initialize");
	}
}

void BaseObject::RemoveAllComponents()
{
	while (m_Components.size() > 0)
	{
		// Delete objects from the back to avoid elements of the vector shifting forward after each delete
		m_Components.back()->Cleanup();
		m_Components.pop_back();
	}
}


void BaseObject::SetEnabled(bool aIsEnabled)
{
	m_Enabled = aIsEnabled;
}

bool BaseObject::IsEnabled()
{
	return m_Enabled;
}


void BaseObject::SetParentScene(Scene* aScene)
{
	m_ParentScene = aScene;
}

Scene* BaseObject::GetParentScene()
{
	return m_ParentScene;
}