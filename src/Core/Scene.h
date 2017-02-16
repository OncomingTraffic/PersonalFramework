#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <memory>
#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

class RenderingManager;
class PhysicsManager;
class LightManager;
class BaseObject;
class GameObject;
class PhysicsObject;
class Behaviour;

class Scene
{
public:
	Scene(std::string tag);
	virtual ~Scene();

	// Load the scene and any objects within the scene
	virtual float Load() = 0;

	virtual void Cleanup();

	// Update any Behaviour components and step the Physics World
	virtual void Update(float delta);

	//TODO: Maybe move this to the Camera class
	virtual void Draw();

	std::string GetTag();

	bool IsLoaded();

	//TODO: Should this be shared_ptr?
	std::vector<std::shared_ptr<BaseObject>> GetBaseObjects();

	// Add an existing BaseObject to this scene
	void AddBaseObject(std::shared_ptr<BaseObject> object);

	//TODO: See if a raw pointer should be used instead
	std::shared_ptr<GameObject> CreateGameObject(glm::vec3 position = glm::vec3(0, 0, 0), glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true, std::string tag = "");
	std::shared_ptr<PhysicsObject> CreatePhysicsObject(); //TODO: Require all necessary physics data to be passed in

	// Remove a BaseObject from this scene using a pointer to the object
	void RemoveBaseObject(std::shared_ptr<BaseObject> object);

	// Remove a BaseObject from this scene using the object's tag
	void RemoveBaseObject(std::string tag);

	void AddBehaviour(Behaviour* behaviour);
	void RemoveBehaviour(Behaviour* behaviour);

	std::shared_ptr<RenderingManager> GetRenderingManager();

	std::shared_ptr<PhysicsManager> GetPhysicsManager();

	std::shared_ptr<LightManager> GetLightManager();


protected:
	// A tag used to identify the scene
	std::string m_Tag;

	// If the scene has been loaded yet
	bool m_IsLoaded;

	//TODO: Maybe change this name
	std::vector<std::shared_ptr<BaseObject>> m_BaseObjects;

	// All behaviours that are attached to BaseObjects in this scene, so they can be updated all at once
	std::vector<Behaviour*> m_Behaviours;

	std::shared_ptr<RenderingManager> m_RenderingManager;

	//TODO: See if this should be moved, since not every scene will use physics
	std::shared_ptr<PhysicsManager> m_PhysicsManager;

	std::shared_ptr<LightManager> m_LightManager;
};



#endif SCENE_H