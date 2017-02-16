#include "Scene.h"
#include "../RenderingManager.h"
#include "../PhysicsManager.h"
#include "../Core/LightManager.h"
#include "../BaseObject.h"
#include "GameObject.h"
#include "PhysicsObject.h"
#include "../Behaviour.h"
#include <SDL.h>


Scene::Scene(std::string aTag) : m_Tag(aTag), m_IsLoaded(false), m_RenderingManager(nullptr), m_LightManager(nullptr), m_PhysicsManager(nullptr)
{
	//
}

Scene::~Scene()
{
	m_BaseObjects.clear();
	m_RenderingManager.reset();
	m_PhysicsManager.reset();
}


void Scene::Cleanup()
{
	//
}

void Scene::Update(float aDelta)
{
	//TODO: Should this be a iterator loop?
	for (unsigned int i = 0; i < m_Behaviours.size(); ++i)
	{
		m_Behaviours.at(i)->Update(aDelta);
	}

	if (m_PhysicsManager != nullptr)
	{
		m_PhysicsManager->Update(aDelta);
	}
}

void Scene::Draw()
{
	//
}

std::string Scene::GetTag()
{
	return m_Tag;
}

bool Scene::IsLoaded()
{
	return m_IsLoaded;
}

//TODO: Should this return a const reference? It seems that returning a vector by value is optimized as of C++11
std::vector<std::shared_ptr<BaseObject>> Scene::GetBaseObjects()
{
	return m_BaseObjects;
}

void Scene::AddBaseObject(std::shared_ptr<BaseObject> aObject)
{
	if (aObject != nullptr)
	{
		aObject->SetParentScene(this);

		if (!aObject->WasInitialized())
		{
			if (aObject->Initialize())
			{
				m_BaseObjects.push_back(aObject);
			}
			else
			{
				SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "BaseObject could not initialize\n");
			}
		}
		else
		{
			m_BaseObjects.push_back(aObject);
		}
		
	}
}

std::shared_ptr<GameObject> Scene::CreateGameObject(glm::vec3 aPosition, glm::quat aRotation, glm::vec3 aScale, bool aIsEnabled, std::string aTag)
{
	std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(aPosition, aRotation, aScale, aIsEnabled, aTag);

	gameObject->SetParentScene(this);

	if (!gameObject->Initialize())
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "GameObject could not initialize!");
		gameObject.reset();
		return nullptr;
	}
	
	m_BaseObjects.push_back(gameObject);

	return gameObject;
}

std::shared_ptr<PhysicsObject> Scene::CreatePhysicsObject()
{
	std::shared_ptr<PhysicsObject> physicsObject = std::make_shared<PhysicsObject>();

	physicsObject->SetParentScene(this);

	if (!physicsObject->Initialize())
	{
		SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_WARN, "PhysicsObject could not initialize!");
		physicsObject.reset();
		return nullptr;
	}

	m_BaseObjects.push_back(physicsObject);

	return physicsObject;
}

void Scene::RemoveBaseObject(std::shared_ptr<BaseObject> object)
{
	//
}

void Scene::RemoveBaseObject(std::string tag)
{
	//
}

void Scene::AddBehaviour(Behaviour* aBehaviour)
{
	m_Behaviours.push_back(aBehaviour);
}

void Scene::RemoveBehaviour(Behaviour* aBehaviour)
{
	std::vector<Behaviour*>::iterator index = std::find(m_Behaviours.begin(), m_Behaviours.end(), aBehaviour);
	if (index != m_Behaviours.end())
	{
		m_Behaviours.erase(index);
	}
}


std::shared_ptr<RenderingManager> Scene::GetRenderingManager()
{
	return m_RenderingManager;
}

std::shared_ptr<PhysicsManager> Scene::GetPhysicsManager()
{
	return m_PhysicsManager;
}

std::shared_ptr<LightManager> Scene::GetLightManager()
{
	return m_LightManager;
}