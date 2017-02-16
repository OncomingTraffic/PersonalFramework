#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "../BaseObject.h"
#include <glm\gtc\matrix_transform.hpp>


//TODO: Encapsulate all framework code in a namespace


class Component;

/*
Inherits from BaseObject, using glm structs for transforms
*/
class GameObject : public BaseObject
{
public:

	GameObject();
	GameObject(glm::vec3 position, glm::quat rotation = glm::quat(), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true, std::string tag = "");
	GameObject(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), bool isEnabled = true, std::string tag = "");
	virtual ~GameObject();

	virtual bool Initialize();

	virtual void Update(double delta);


	virtual void SetPosition(glm::vec3 position);
	virtual void SetPosition(btVector3 &position);
	virtual void SetPositionX(float positionX);
	virtual void SetPositionY(float positionY);
	virtual void SetPositionZ(float positionZ);
	virtual glm::vec3 GetPosition();
	virtual btVector3 GetBTPosition();

	virtual void SetRotation(glm::quat rotation);
	virtual void SetRotation(glm::vec3 eulerAngles);
	virtual void SetRotation(btQuaternion &rotation);
	virtual void Rotate(float angle, glm::vec3 axis);
	virtual glm::quat GetRotation();
	virtual btQuaternion GetBTRotation();

	virtual void SetScale(glm::vec3 scale);
	virtual void SetScale(btVector3 &scale);
	virtual void SetScaleX(float scaleX);
	virtual void SetScaleY(float scaleY);
	virtual void SetScaleZ(float scaleZ);
	virtual glm::vec3 GetScale();
	virtual btVector3 GetBTScale();

	glm::mat4x4 GetModelMatrix();

	

	//template<class T> std::shared_ptr<T> AddComponent();

	// Add a component to this GameObject. The component will be initialized within this function
	//void AddComponent(std::shared_ptr<Component> component);


protected:

	//TODO: Consider making a Transform struct to store this data in

	glm::vec3 m_Position;

	glm::quat m_Rotation;

	glm::vec3 m_Scale;

	glm::mat4x4 m_ModelMatrix;
	bool m_ModelMatrixIsDirty;

	


};



#endif GAMEOBJECT_H