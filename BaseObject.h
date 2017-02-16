#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <GL\glew.h>
#include <gl\GLU.h>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <memory>
#include <vector>
#include <string>
#include "btBulletDynamicsCommon.h"


class Scene;
class Component;


//TODO: Change this name
// An abstract class that represents objects that have a world transform
// This class itself does not have the transform, only the method declarations for getting the transforms
// Variables must me added in inheriting classes, and get and set methods must be defined
class BaseObject
{
public:

	BaseObject();
	BaseObject(std::string tag);
	virtual ~BaseObject();

	virtual bool Initialize();
	virtual void Cleanup();

	virtual void SetPosition(glm::vec3 position) = 0;
	virtual void SetPosition(btVector3 &position) = 0;
	virtual void SetPositionX(float positionX) = 0;
	virtual void SetPositionY(float positionY) = 0;
	virtual void SetPositionZ(float positionZ) = 0;
	virtual glm::vec3 GetPosition() = 0;
	virtual btVector3 GetBTPosition() = 0;

	virtual void SetRotation(glm::quat rotation) = 0;
	virtual void SetRotation(btQuaternion &rotation) = 0;
	virtual void SetRotation(glm::vec3 eulerAngles) = 0;
	virtual void Rotate(float angle, glm::vec3 axis) = 0;
	virtual glm::quat GetRotation() = 0;
	virtual btQuaternion GetBTRotation() = 0;

	virtual void SetScale(glm::vec3 scale) = 0;
	virtual void SetScale(btVector3 &scale) = 0;
	virtual void SetScaleX(float scaleX) = 0;
	virtual void SetScaleY(float scaleY) = 0;
	virtual void SetScaleZ(float scaleZ) = 0;
	virtual glm::vec3 GetScale() = 0;
	virtual btVector3 GetBTScale() = 0;

	//TODO: Update all child objects as well
	virtual glm::mat4x4 GetModelMatrix() = 0;

	//template<class T> std::shared_ptr<T> AddComponent();

	void SetTag(std::string tag);
	std::string GetTag();

	// Check if this object has been initialized already
	bool WasInitialized();

	// Add a component to this Basebject. The component will be initialized within this method
	void AddComponent(std::shared_ptr<Component> component);

	// Remove a component from this BaseObject. Calls the Cleanup method of the component
	void RemoveComponent(std::shared_ptr<Component> component);

	// Remove all components from this BaseObject. Calls the Cleanup method of each component
	void RemoveAllComponents();

	void SetParentScene(Scene* scene);
	Scene* GetParentScene();

	virtual void SetEnabled(bool isEnabled);
	virtual bool IsEnabled();


protected:

	// A string used to identify this object
	std::string m_Tag;

	// If the Initialize method has already been called on this object
	// This is used by PhysicsObject sub class to set it up with the PhysicsManager
	bool m_Initialized;

	std::vector<std::shared_ptr<BaseObject>> m_Children;

	std::vector<std::shared_ptr<Component>> m_Components;

	// The scene that this object belongs to
	Scene* m_ParentScene;

	bool m_Enabled;
};


#endif //BASEOBJECT_H