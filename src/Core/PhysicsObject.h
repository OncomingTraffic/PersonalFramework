#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "../BaseObject.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"

#pragma region Forward Declarations 

class PhysicsManager;

#pragma endregion


enum class CollisionObjectType
{
	CO_NONE,
	CO_RIGIDBODY
};


/*
Inherits from BaseObject, using the transforms of Bullet's btCollisionObject
*/
class PhysicsObject : public BaseObject
{
public:

	PhysicsObject();
	PhysicsObject(glm::vec3 position, glm::vec3 rotation = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), float mass = 1.0f, glm::vec3 velocity = glm::vec3(0, 0, 0), glm::vec3 angularVelocity = glm::vec3(0, 0, 0), bool isEnabled = true);
	virtual ~PhysicsObject();

	virtual bool Initialize();
	virtual void Cleanup();

	virtual void SetPosition(glm::vec3 position);
	virtual void SetPosition(btVector3 &position);
	virtual void SetPositionX(float positionX);
	virtual void SetPositionY(float positionY);
	virtual void SetPositionZ(float positionZ);
	virtual glm::vec3 GetPosition();
	virtual btVector3 GetBTPosition();

	virtual void SetRotation(glm::quat rotation);
	virtual void SetRotation(btQuaternion &rotation);
	virtual void SetRotation(glm::vec3 eulerAngles);
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

	// Set the PhysicsManager that this object is part of
	// Should only be called when adding this object to the Scene, and not manually
	void SetPhysicsManager(PhysicsManager* physicsManager);

	//TODO: Possibly change this to something other than shared_ptr
	void SetCollisionObject(std::shared_ptr<btCollisionObject> collisionObject);
	std::shared_ptr<btCollisionObject> GetCollisionObject();

	// Set the Collision Object as a Rigidbody. Use initialTransform to determine the transorm of this PhysicsObject, since it's transforms are based on the Collision Object
	void SetRigidbody(const float &mass, std::shared_ptr<btCollisionShape> collisionShape, const btVector3 &localInertia = btVector3());
	void SetRigidbody(const float &mass, std::shared_ptr<btCollisionShape> collisionShape, const btTransform &initialTransform, const btVector3 &localInertia = btVector3());

	// Returns the btRigidbody if this object is using a rigidbody, otherwise returns nullptr
	// PhysicsObjects may use other types of collision objects
	btRigidBody* GetRigidbody();

	btMotionState* GetMotionState();

protected:

	std::shared_ptr<btCollisionObject> m_CollisionObject;

	//float m_Mass;

	std::shared_ptr<btCollisionShape> m_CollisionShape;

	std::unique_ptr<btDefaultMotionState> m_MotionState;

	// The PhysicsManager that this object is a part of
	PhysicsManager* m_PhysicsManager;

	CollisionObjectType m_CollisionObjectType;
};


#endif PHYSICSOBJECT_H